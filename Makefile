CC=gcc
CFLAGS=-I.
LDFLAGS=-pthread

all: clean server client

server: server.o server_messages.o server_gamelogic.o
	$(CC) $(LDFLAGS) server.o server_messages.o server_gamelogic.o -o server

server.o:
	$(CC)  -c server.c

server_messages.o:
	$(CC)  -c server_messages.c

server_gamelogic.o:
	$(CC)  -c server_gamelogic.c

client: client.o client_messages.o
	$(CC)  $(LDFLAGS) client.o client_messages.o -o client

client.o:
	$(CC) -c client.c

client_messages.o:
	$(CC) -c client_messages.c

clean:
	rm -f *o
