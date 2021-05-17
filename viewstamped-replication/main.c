#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <uv.h>

#include "util.h"

#define VR_SOCKET_BACKLOG 256
/*
 * Each message consists of 4 parts 1 byte per each:
 * - command number
 * - register number
 * - value
 * - terminating character usually being newline
 */
#define VR_MSG_SIZE 4

struct control_connection {
	int in_cur;
	char in_cmd[VR_MSG_SIZE];
	char out_cmd[VR_MSG_SIZE];
	uv_pipe_t handle;
};

static uv_loop_t *mainloop = NULL;
static uv_pipe_t *control_handle = NULL;
static uv_signal_t *sigint_handle = NULL;
static char database[256];

static void allocate_control_connection_read_buffer(uv_handle_t *handle, size_t proposed_size, uv_buf_t *buf);
static void on_control_connection_write(uv_write_t *wreq, int status);
#define stop_reading(x) verbose_stop_reading(x, __FILE__, __LINE__)
static inline bool verbose_stop_reading(uv_stream_t *stream, const char *file, int line);
static void on_control_connection_close(uv_handle_t *handle);
static void on_control_connection_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);
static void on_new_control_connection(uv_stream_t *in_stream, int status);
static void on_handle_close(uv_handle_t *handle);
static void close_application(void);
static void on_sigint(uv_signal_t *signal, int signum);

static void allocate_control_connection_read_buffer(
	uv_handle_t *handle,
	size_t proposed_size,
	uv_buf_t *buf
) {
	struct control_connection *cconn = (struct control_connection *)handle->data;
	if (VR_MSG_SIZE - 1 == cconn->in_cur) {
		buf->base = NULL;
		buf->len = 0;
		return;
	}
	buf->base = cconn->in_cmd + cconn->in_cur;
	buf->len = VR_MSG_SIZE - cconn->in_cur;
}

static void on_control_connection_write(uv_write_t *wreq, int status) {
	int r;
	struct control_connection *cconn = (struct control_connection *)wreq->handle->data;

	if (status < 0) {
		LOG_WARN("Failed to respond to control request: %s", uv_strerror(status));
		uv_close((uv_handle_t *)wreq->handle, on_control_connection_close);
	} else {
		cconn->in_cur = 0;
		if ((r = uv_read_start(
			(uv_stream_t *)wreq->handle,
			allocate_control_connection_read_buffer,
			on_control_connection_read)) < 0
		) {
			LOG_WARN("Failed to read control request: %s", uv_strerror(r));
			uv_close((uv_handle_t *)wreq->handle, on_control_connection_close);
		}
	}
	free(wreq);
}

static inline bool verbose_stop_reading(uv_stream_t *stream, const char *file, int line) {
	int r;
	if ((r = uv_read_stop(stream)) < 0) {
		LOG_WARN("Failed to stop reading from stream: %s", uv_strerror(r));
		return false;
	}
	return true;
}

static void on_control_connection_close(uv_handle_t *handle) {
	struct control_connection *cconn = (struct control_connection *)handle->data;
	if (cconn != NULL) {
		free(cconn);
	}
}

static void on_control_connection_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
	int r;
	uv_write_t *wreq = NULL;
	uv_buf_t out_bufs[1];
	struct control_connection *cconn = (struct control_connection *)stream->data;

	if (nread == UV_EOF) {
		(void)stop_reading(stream);
		uv_close((uv_handle_t *)stream, on_control_connection_close);
		return;
	} else if (nread < 0) {
		LOG_WARN("Error while reading from control connection: %s", uv_strerror(nread));
		if (stop_reading(stream)) {
			uv_close((uv_handle_t *)stream, on_control_connection_close);
			return;
		}
	} else if (nread == 0 && cconn->in_cur != VR_MSG_SIZE - 1) {
		return;
	}

	(void)stop_reading(stream);
	switch (cconn->in_cmd[0]) {
		case 'e':
			LOG_DEBUG("Close command issued via control connection. Shutting down...");
			close_application();
			uv_close((uv_handle_t *)stream, on_control_connection_close);
			break;
		case 'p':
			LOG_DEBUG("Putting new value into database");
			database[(unsigned int)cconn->in_cmd[1]] = cconn->in_cmd[2];
			cconn->out_cmd[0] = 's';
			cconn->out_cmd[1] = cconn->in_cmd[1];
			cconn->out_cmd[2] = cconn->in_cmd[2];
			cconn->out_cmd[3] = '\n';
			out_bufs[0].base = cconn->out_cmd;
			out_bufs[0].len = VR_MSG_SIZE;
			wreq = xmalloc(sizeof(uv_write_t));
			if ((r = uv_write(wreq, stream, out_bufs, 1, on_control_connection_write)) < 0) {
				LOG_WARN("Failed to respond to control request");
				uv_close((uv_handle_t *)stream, on_control_connection_close);
				free(wreq);
			}
			break;
		case 'g':
			LOG_DEBUG("Getting value from database");
			cconn->out_cmd[0] = 'v';
			cconn->out_cmd[1] = cconn->in_cmd[1];
			cconn->out_cmd[2] = database[(int)cconn->in_cmd[2]];
			cconn->out_cmd[3] = '\n';
			out_bufs[0].base = cconn->out_cmd;
			out_bufs[0].len = VR_MSG_SIZE;
			wreq = xmalloc(sizeof(uv_write_t));
			if ((r = uv_write(wreq, stream, out_bufs, 1, on_control_connection_write)) < 0) {
				LOG_WARN("Failed to respond on control request");
				uv_close((uv_handle_t *)stream, on_control_connection_close);
				free(wreq);
			}
			break;
		default:
			LOG_WARN("Undefined command issued via control connection: %.3s", cconn->in_cmd);
			uv_close((uv_handle_t *)stream, on_control_connection_close);
			break;
	}
}

static void on_new_control_connection(uv_stream_t *in_stream, int status) {
	int r;
	struct control_connection *cconn;

	LOG_DEBUG("New connection arrived on control command socket");
	cconn = xmalloc(sizeof(struct control_connection));
	if ((r = uv_pipe_init(mainloop, &cconn->handle, 0)) < 0) {
		LOG_DEBUG("Failed to initialize client pipe handle");
		goto err_with_cconn;
	}
	if ((r = uv_accept(in_stream, (uv_stream_t *)&cconn->handle)) < 0) {
		LOG_DEBUG("Failed to accept client conenction");
		goto err_with_cconn;
	}
	cconn->in_cur = 0;
	cconn->handle.data = cconn;
	if ((r = uv_read_start(
		(uv_stream_t *)&cconn->handle,
		allocate_control_connection_read_buffer,
		on_control_connection_read)) < 0
	) {
		LOG_WARN("Failed to start reading from stream: %s", uv_strerror(r));
		goto err_with_cconn;
	}
	return;

err_with_cconn:
	free(cconn);
}

static void on_handle_close(uv_handle_t *handle) {
	return;
}

static void close_application(void) {
	uv_close((uv_handle_t *)control_handle, on_handle_close);
	uv_close((uv_handle_t *)sigint_handle, on_handle_close);
}

static void on_sigint(uv_signal_t *signal, int signum) {
	LOG_DEBUG("Received SIGINT. Stopping application...");
	close_application();
}

int main(int argc, char **argv) {
	int r;

	memset(database, '0', 256);
	mainloop = xmalloc(sizeof(uv_loop_t));
	if ((r = uv_loop_init(mainloop)) < 0) {
		LOG_ERROR("Failed to initialize main loop: %s", uv_strerror(r));
	}

	control_handle = xmalloc(sizeof(uv_pipe_t));
	if ((r = uv_pipe_init(mainloop, control_handle, 0)) < 0) {
		LOG_ERROR("Failed to initialize input command socket: %s", uv_strerror(r));
	}
	if ((r = uv_pipe_bind(control_handle, "./control")) < 0) {
		LOG_ERROR("Failed to bind input command socket: %s", uv_strerror(r));
	}
	if ((r = uv_listen((uv_stream_t *)control_handle, VR_SOCKET_BACKLOG, on_new_control_connection)) < 0) {
		LOG_ERROR("Failed to listen for control commands: %s", uv_strerror(r));
	}

	sigint_handle = xmalloc(sizeof(uv_signal_t));
	if ((r = uv_signal_init(mainloop, sigint_handle)) < 0) {
		LOG_ERROR("Failed to initialize SIGHUP handler: %s", uv_strerror(r));
	}
	if ((r = uv_signal_start_oneshot(sigint_handle, on_sigint, SIGINT)) < 0) {
		LOG_ERROR("Failed to register SIGINT callback: %s", uv_strerror(r));
	}

	if ((r = uv_run(mainloop, UV_RUN_DEFAULT)) < 0) {
		LOG_ERROR("Main loop failed: %s", uv_strerror(r));
	}
	if ((r = uv_loop_close(mainloop)) < 0) {
		LOG_ERROR("Failed to close main loop: %s", uv_strerror(r));
	}

	free(mainloop);
	free(control_handle);
	free(sigint_handle);
	return 0;
}
