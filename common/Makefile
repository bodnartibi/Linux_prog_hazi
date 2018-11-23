CC=g++
CFLAGS=-I.

all: clean bid.o

bid.o:
	$(CC) $(CFLAGS) -c bid.c

check:
	cd tests && make all

clean:
	rm -rf *.o

