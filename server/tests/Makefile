CC=g++
CFLAGS=-I. -I../ -I../../common

all: client run

run:
	./test_client

client: client.o
	$(CC) $(CFLAGS) test_client.o ../client.o ../../common/bid.o -o test_client

client.o:
	$(CC)  $(CFLAGS) -c test_client.c
