CC=gcc
CFLAGS=-I.
LDFLAGS=-pthread

all: clean server

server: server.o server_messages.o server_gamelogic.o
	$(CC) $(LDFLAGS) server.o server_messages.o server_gamelogic.o -o server

server.o:
	$(CC)  -c -Wall server.c

server_messages.o:
	$(CC)  -c -Wall server_messages.c

server_gamelogic.o:
	$(CC)  -c -Wall server_gamelogic.c


clean:
	rm -f *o
