CC = gcc
CFLAGS = -Wall -g
LIBS =

all: main.o
	$(CC) $(CFLAGS) -o main $^ $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f `find ./ -name *.o`
	rm -f main
