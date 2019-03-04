#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <sys/queue.h>
#include <stdlib.h>

typedef struct connection_s* ConnectionPtr;

ConnectionPtr connection_new(void);
void connection_destroy(ConnectionPtr connection);
void *connection_allocate_buffer(ConnectionPtr connection, size_t size);
char *connection_get_message(ConnectionPtr connection);
void connection_cycle_buffers(ConnectionPtr connection);

#endif
