CC=gcc
CFLAGS=-I. -I../common
#LDFLAGS=-pthread

all: clean server

server: server.o server_messages.o server_gamelogic.o
	$(CC) $(CFLAGS) $(LDFLAGS) server.o server_messages.o server_gamelogic.o -o server

server.o:
	$(CC)  $(CFLAGS) -c server.c

server_messages.o:
	$(CC)  $(CFLAGS) -c server_messages.c

server_gamelogic.o:
	$(CC)  $(CFLAGS) -c server_gamelogic.c


clean:
	rm -f *o
