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

#include "messages.h"
#include "client.h"
	
struct server_prop_client_msg 	clients;
struct server_newroll_msg				act_roll;
struct server_prop_bid_msg			act_bid;
struct who_won_msg							won;
struct your_ID									my_ID;
struct client_reg_msg						reg;
struct client_game_msg					game_msg;
struct client_ready							ready_msg;

int process_client_message(int phase, void* msg, int msglen) {
	int msg_ID ;
	msg_ID = *(int*)msg;
	int res;
	int index;
	int my_face;
	int my_quan;
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
	
	switch(msg_ID){

		case YOUR_ID:
			my_ID = *(struct your_ID*)msg;
			printf("Client: my ID %d\n",my_ID.client_ID);
			printf("Client: send my name: %s\n",	my_name);

			reg.msgID = REG_CLIENT;
			reg.client_ID = my_ID.client_ID;
			strcpy(&reg.name[0], my_name);
							
			res = write(sockfd, (void*)&reg, sizeof(struct client_reg_msg));
			if(res < 0){
				fprintf(stderr,"Client: Hiba: write %d %s. \n",res,strerror(errno));
			}

			//TODO ez csak teszt hogy azonnal küldünk readyt
			printf("Client: send ready\n");
			ready_msg.msgID = CLIENT_READY;
			ready_msg.client_ID = my_ID.client_ID;
			ready_msg.ready = true;

			//TODO miért csak sleepel megy hogy átmegy ez az üzenet is?
			sleep(2);
			res = write(sockfd, (void*)&ready_msg, sizeof(ready_msg));
			if(res < 0){
				fprintf(stderr,"Client: Hiba: write %d %s. \n",res,strerror(errno));
			}

			break;	
		
		case PROP_BID:
			act_bid = *(struct server_prop_bid_msg*)msg;
			printf("Client: Actual bid: face %d quantity %d your turn? %d\n", act_bid.bid_face, act_bid.bid_quantity, act_bid.your_turn);
			
			//TODO külön kezelni a user inputot, hogy bármikor tudjunk írni
			scanf("%d %d",&my_face, &my_quan);
			//TODO csak nagyobb lehet a tét
			printf("Client: New bid: face %d quantity %d\n", my_face, my_quan);

			game_msg.msgID = NEW_BID;
			game_msg.client_ID = my_ID.client_ID;
			game_msg.bid_face = my_face;
			game_msg.bid_quantity = my_quan;
	
			res = write(sockfd, (void*)&game_msg, sizeof(game_msg));
			if(res < 0){
				fprintf(stderr,"Client: Hiba: write %d %s. \n",res,strerror(errno));
			}
			break;

		case NEW_DICE_ROLL:
			act_roll = *(struct server_newroll_msg*)msg;
		
			printf("Client: Your dices: ");
			for(index = 0; index < MAX_DICE_NUM; index++){
				if(act_roll.dices[index] > 0)
				printf("%d ",act_roll.dices[index]);
			}
			printf("\n");
	
			break;

		default:
			fprintf(stderr,"Hiba: Client: invalid messagetype %d msgID: %d\n", phase, msg_ID);
			break;
	}
	printf("Client: get message ID %d\n",msg_ID);
	return 0;
//	new_dices(dices);
}
