hazimake: server.c server_gamelogic.c
	gcc -o server server_gamelogic.c server.c -I. 
