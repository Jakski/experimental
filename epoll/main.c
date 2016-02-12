#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10

/*
 * Program requires pipe 'pipe1' to exist. It has to have some input first,
 * so program can start. Executable simply accept input on stdin and 'pipe1',
 * to print it to stdout.
 *
 * In first TTY:
 * $> mkfifo pipe1
 * $> make && ./main
 *
 * In second TTY:
 * $> echo 'something' > pipe1
 */

int main(int argc, char **argv)
{
    int p1 = open("pipe1", O_RDONLY);

    int hub = epoll_create1(0);
    struct epoll_event ev, events[MAX_EVENTS];

    /*Set event to be edge-triggered*/
    ev.events = EPOLLIN | EPOLLET;

    ev.data.fd = 0;
    epoll_ctl(hub, EPOLL_CTL_ADD, 0, &ev);
    ev.data.fd = p1;
    epoll_ctl(hub, EPOLL_CTL_ADD, p1, &ev);

    char buff[10];
    for (;;)
    {
        int n, nfds = epoll_wait(hub, events, MAX_EVENTS, -1);
        for (n = 0; n < nfds; ++n)
        {
            memset(&buff, 0, sizeof(char) * 10);
            if (events[n].data.fd == 0)
            {
                read(0, &buff, 10);
                buff[9] = '\0';
                printf("%s", buff);
            }
            if (events[n].data.fd == p1)
            {
                read(p1, &buff, 10);
                buff[9] = '\0';
                printf("%s", buff);
            }
            fflush(stdout);
        }
    }

    close(hub);
    close(p1);
    return 0;
}
