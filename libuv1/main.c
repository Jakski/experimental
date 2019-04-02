#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <uv.h>

#include "config.h"
#include "util.h"
#include "connection.h"

static uv_loop_t *main_loop;
static uv_tcp_t server;

static void on_close(uv_handle_t *handle)
{
    free(handle);
}

static void on_new_connection(uv_stream_t *server, int status)
{
    int err;
    if(status < 0)
    {
        LOG_ERROR(uv_strerror(status));
        return;
    }
    uv_tcp_t *client = malloc(sizeof(uv_tcp_t));
    client->data = malloc(sizeof(int));
    *(int *)client->data = 0;
    err = uv_tcp_init(main_loop, client);
    if(err)
    {
        LOG_ERROR(uv_strerror(err));
        return;
    }
    if(uv_accept(server, (uv_stream_t *)client) == 0)
    {
        LOG_INFO("Connection accepted");
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        err = uv_tcp_getpeername(client, (struct sockaddr *)&address, &addrlen);
        if(err < 0)
        {
            LOG_ERROR("Failed to retrieve peer name");
            return;
        }
        LOG_INFO(
            "Peer address: %s:%d",
            inet_ntoa(address.sin_addr),
            ntohs(address.sin_port)
        );
        /*err = uv_read_start((uv_stream_t *)client, allocate_buffer, on_read);*/
        /*if(err)*/
        /*{*/
            /*LOG_ERROR(uv_strerror(err));*/
            /*uv_close((uv_handle_t *)client, on_close);*/
        /*}*/
    } else {
        LOG_ERROR("Failed to accept connection");
    }
}

int main(int argc, char **argv)
{
    LOG_INFO("Starting chat-server...");
    LOG_INFO("Version: %s", VERSION);

    struct sockaddr_in address;
    int err;
    main_loop = uv_default_loop();
    if(!main_loop)
    {
        LOG_ERROR("Failed to allocate main loop");
        abort();
    }
    err = uv_tcp_init(main_loop, &server);
    if(err) goto uv_error;
    err = uv_ip4_addr(ADDRESS, PORT, &address);
    if(err) goto uv_error;
    err = uv_tcp_bind(&server, (const struct sockaddr *)&address, 0);
    if(err) goto uv_error;
    err = uv_listen((uv_stream_t *)&server, CONNECTION_BACKLOG, on_new_connection);
    if(err) goto uv_error;
    uv_run(main_loop, UV_RUN_DEFAULT);

    LOG_INFO("Shutting down chat-server...");
    uv_loop_close(main_loop);
    return 0;

uv_error:
    LOG_ERROR(uv_strerror(err));
    abort();
}
