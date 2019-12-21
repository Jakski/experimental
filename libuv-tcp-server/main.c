#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <uv.h>

#include "config.h"

#define LOG_ERROR(...) \
  do { \
    fprintf(stderr, "ERROR:%s:%d: ", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n"); \
  } while(0)

#define LOG_INFO(...) \
  do { \
    fprintf(stdout, "INFO:%s:%d: ", __FILE__, __LINE__); \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, "\n"); \
  } while(0)

typedef struct {
  int id;
  char state;
} session_t;

typedef struct {
  uv_write_t req;
  uv_buf_t buf;
} write_req_t;

static uv_loop_t *loop;

void alloc_buff(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
  /* Buffer one character and newline from netcat. */
  buf->base = (char *)malloc(sizeof(char) * 2);
  buf->len = sizeof(char) * 2;
}

void on_close(uv_handle_t *handle)
{
  session_t *session = (session_t *)handle->data;
  LOG_INFO("Connection %d closed", session->id);
  free(handle->data);
  free(handle);
}

void on_write(uv_write_t *req, int status)
{
  if (status)
    LOG_ERROR("Write error %s", uv_strerror(status));
  write_req_t *wr = (write_req_t *)req;

  free(wr->buf.base);
  free(req);
}

void on_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
  if (nread > 0)
  {
    session_t *session = client->data;
    LOG_INFO("Connection %d transitioning from %c to %c", session->id, session->state, *(char *)buf->base);
    session->state = *(char *)buf->base;
    write_req_t *req = (write_req_t *)malloc(sizeof(write_req_t));
    req->buf = uv_buf_init(buf->base, nread);
    uv_write((uv_write_t *)req, client, &req->buf, 1, on_write);
    return;
  }
  if (nread < 0)
  {
    if (nread != UV_EOF)
      LOG_ERROR("Read error %s", uv_err_name(nread));
    uv_close((uv_handle_t *)client, on_close);
  }

  free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status)
{
  if (status == -1)
    return;

  uv_tcp_t *client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
  client->data = malloc(sizeof(session_t));
  session_t *session = (session_t *)client->data;
  session->id = rand();
  session->state = 'a';
  uv_tcp_init(loop, client);
  if (uv_accept(server, (uv_stream_t *)client) == 0)
  {
    uv_read_start((uv_stream_t *)client, alloc_buff, on_read);
  } else {
    uv_close((uv_handle_t *) client, NULL);
  }
  LOG_INFO("New connection with ID: %d", session->id);
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  loop = uv_default_loop();

  uv_tcp_t server;
  uv_tcp_init(loop, &server);

  struct sockaddr_in addr;
  uv_ip4_addr(TCP_SERVER_ADDRESS, TCP_SERVER_PORT, &addr);
  uv_tcp_bind(&server, (const struct sockaddr *)&addr, 0);
  int r;
  if ((r = uv_listen((uv_stream_t *)&server, TCP_SERVER_CONNECTION_BACKLOG, on_new_connection)))
  {
    LOG_ERROR("Listen error %s", uv_err_name(r));
    return 1;
  }
  LOG_INFO("Listening on %s:%d", TCP_SERVER_ADDRESS, TCP_SERVER_PORT);

  return uv_run(loop, UV_RUN_DEFAULT);
}
