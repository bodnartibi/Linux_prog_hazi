CC=g++
CFLAGS=-I. -I../

all: bid run

run:
	./test_bid

bid: bid.o
	$(CC) $(CFLAGS) test_bid.o ../bid.o -o test_bid
bid.o:
	$(CC)  $(CFLAGS) -c test_bid.c
