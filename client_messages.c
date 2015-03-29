#include "messages.h"
#include <errno.h>
#include <stdio.h>
	
struct server_prop_client_msg* 	clients;
struct server_newroll_msg*			act_roll;
struct server_prop_bid_msg*			act_bid;
struct who_won_msg*							won;
struct your_ID*									my_ID;

int process_client_message(int phase, void* msg, int msglen) {
	int* msg_ID ;
	msg_ID = (int*)msg;

	// ------------------------------
	// check if ID matches with phase
	// TODO ez még gány
//	if( ((*msg_ID >= 0x10) && (phase != 0)) || 
//			((*msg_ID < 0x10 || *msg_ID >= 0x30) && (phase != 1)) || 
//			((*msg_ID <= 0x30) && (phase != 2))
//		){
//		fprintf(stderr,"Hiba: Client: invalid message Phase %d msgID: %d\n", phase, *msg_ID);
//		return -1;
//	}
	
	switch(*msg_ID){
		case YOUR_ID:
			my_ID = (struct your_ID*)msg;
			printf("Client: my ID %d\n",my_ID->client_ID);
			break;	
		default:
			fprintf(stderr,"Hiba: Client: invalid messagetype %d msgID: %d\n", phase, *msg_ID);
			break;
	}
	printf("Client: get message ID %d\n",*msg_ID);
	return 0;
//	new_dices(dices);
}
