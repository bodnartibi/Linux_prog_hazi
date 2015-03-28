CC=gcc
CFLAGS=-I.
LDFLAGS=-pthread

hazimake: server.c server_gamelogic.c
	$(CC) $(LDFLAGS) -o server server_gamelogic.c server.c server_gamelogic_test.c
	$(CC) $(LDFLAGS) -o client client.c
