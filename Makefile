CC=gcc
CFLAGS=-I.
#LDFLAGS=-pthread

all: clean server

server: server.o server_messages.o server_gamelogic.o
	$(CC) $(LDFLAGS) server.o server_messages.o server_gamelogic.o -o server

server.o:
	$(CC)  -c server.c

server_messages.o:
	$(CC)  -c server_messages.c

server_gamelogic.o:
	$(CC)  -c server_gamelogic.c


clean:
	rm -f *o
