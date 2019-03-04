#include <string.h>

#include "connection.h"

typedef struct buffer_entry_s {
    size_t size;
    void *payload;
    TAILQ_ENTRY(buffer_entry_s) entries;
} BufferEntry;

typedef TAILQ_HEAD(buffer_s, buffer_entry_s) Buffer;

struct connection_s {
    Buffer buff;
} Connection;

static void new_buffer_entry(Buffer *buff, size_t size)
{
    BufferEntry *buffentry = malloc(sizeof(BufferEntry));
    buffentry->payload = malloc(size);
    buffentry->size = size;
    TAILQ_INSERT_TAIL(buff, buffentry, entries);
    memset(buffentry->payload, 0, size);
}

static void remove_buffer_until(Buffer *buff, BufferEntry *until)
{
    BufferEntry *entry2, *entry1 = TAILQ_FIRST(buff);
    while(entry1 != until)
    {
        entry2 = TAILQ_NEXT(entry1, entries);
        free(entry1->payload);
        free(entry1);
        TAILQ_REMOVE(buff, entry1, entries);
        entry1 = entry2;
    }
}

ConnectionPtr connection_new(void)
{
    ConnectionPtr connection = malloc(sizeof(Connection));
    TAILQ_INIT(&connection->buff);
    return connection;
}

void connection_destroy(ConnectionPtr connection)
{
    remove_buffer_until(&connection->buff, NULL);
    free(connection);
}

void *connection_allocate_buffer(ConnectionPtr connection, size_t size)
{
    new_buffer_entry(&connection->buff, size);
    return TAILQ_LAST(&connection->buff, buffer_s)->payload;
}

char *connection_get_message(ConnectionPtr connection)
{
    BufferEntry *last = TAILQ_LAST(&connection->buff, buffer_s);
    char *found = memchr(last->payload, '\n', last->size);
    if(found == NULL) return NULL;
    BufferEntry *entry;
    size_t size = 0;
    TAILQ_FOREACH(entry, &connection->buff, entries)
    {
        if(entry != last)
        {
            size += entry->size;
        } else {
            for(char *c = entry->payload; c != (char *)entry->payload + (entry->size / sizeof(char)); c++)
            {
                if(c == found) break;
                size += sizeof(char);
            }
        }
    }
    char *message = malloc(size + sizeof(char));
    message[size / sizeof(char)] = '\0';
    char *cur = message;
    TAILQ_FOREACH(entry, &connection->buff, entries)
    {
        if(entry != last)
        {
            memcpy(cur, entry->payload, entry->size);
            cur += entry->size / sizeof(char);
            size -= entry->size;
        } else {
            memcpy(cur, entry->payload, size);
        }
    }
    return message;
}

void connection_cycle_buffers(ConnectionPtr connection)
{
    BufferEntry *last = TAILQ_LAST(&connection->buff, buffer_s);
    char *found = memchr(last->payload, '\n', last->size);
    int size = 0;
    if(found == NULL) return;
    for(char *p = last->payload + (last->size / sizeof(char)) - 1; p != found; p--)
        size++;
    if(size == 0)
    {
        remove_buffer_until(&connection->buff, NULL);
    } else {
        new_buffer_entry(&connection->buff, size);
        last = TAILQ_LAST(&connection->buff, buffer_s);
        memcpy(found, last->payload, size);
        remove_buffer_until(&connection->buff, last);
    }
}
