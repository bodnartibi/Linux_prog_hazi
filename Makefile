CC=gcc
CFLAGS=-I.

hazimake: server.c server_gamelogic.c
	$(CC) -o server server_gamelogic.c server.c server_gamelogic_test.c
	$(CC) -o client client.c
