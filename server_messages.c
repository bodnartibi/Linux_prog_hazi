#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "server_messages.h"

int process_server_message(int phase, void* msg, int msglen, int dices[][MAX_DICE_NUM]) {
	int* msg_ID ;
	msg_ID = (int*)msg;
	
	// ------------------------------
	// check if ID matches with phase
	// TODO ez még gány
	if( ((*msg_ID >= 0x10) && (phase != 0)) || 
			((*msg_ID < 0x10 || *msg_ID >= 0x30) && (phase != 1)) || 
			((*msg_ID <= 0x30) && (phase != 2))
		){
		fprintf(stderr,"Hiba: Server: invalid message Phase %d msgID: %d . \n", phase, *msg_ID);
		return -1;
	}
	
	printf("Server: get message ID %d",*msg_ID);
	return 0;
//	new_dices(dices);
}


