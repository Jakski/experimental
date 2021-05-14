#include <stdlib.h>
#include <uv.h>

#include "util.h"

#define VR_SOCKET_BACKLOG 256

static uv_loop_t *mainloop = NULL;
static uv_pipe_t *control_handle = NULL;
static uv_signal_t *sigint_handle = NULL;

static void on_new_control_command(uv_stream_t *in_stream, int status) {
  LOG_DEBUG("New connection arrived on control command socket");
}

static void on_handle_close(uv_handle_t *handle) {
  return;
}

static void on_sigint(uv_signal_t *signal, int signum) {
  LOG_DEBUG("Received SIGINT. Stopping application...");
  uv_close((uv_handle_t *)control_handle, on_handle_close);
  uv_close((uv_handle_t *)signal, on_handle_close);
}

int main(int argc, char **argv) {
  int r;

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
  if ((r = uv_listen((uv_stream_t *)control_handle, VR_SOCKET_BACKLOG, on_new_control_command)) < 0) {
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
