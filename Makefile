CC=gcc
CFLAGS=-I.
LDFLAGS=-pthread

all: server client

server: server.o server_messages.o server_gamelogic.o
	$(CC) server.o server_messages.o server_gamelogic.o -o server

server.o:
	$(CC) $(LDFLAGS) -c -Wall server.c

server_messages.o:
	$(CC)  -c -Wall server_messages.c

server_gamelogic.o:
	$(CC)  -c -Wall server_gamelogic.c

client:
	$(CC) $(LDFLAGS) -o client client.c client_messages.c

clean:
	rm *o
