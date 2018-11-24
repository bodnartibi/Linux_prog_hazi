CC=g++
CFLAGS=-I. -I../common
#LDFLAGS=-pthread
all: client.o

client.o:
	$(CC)  $(CFLAGS) -c client.c

check:
	cd tests && make all

clean:
	rm -f *o
