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

	char user_buf[256];
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
			
			if(act_bid.your_turn != true){
				break;
			}
			printf("This is your turn.\n Options: \n New bid: bid <face> <quantitiy> \n Challenge: challenge\n");
			//TODO külön kezelni a user inputot, hogy bármikor tudjunk írni
			res = read(STDIN_FILENO, &user_buf, sizeof(user_buf)-1 );
			user_buf[sizeof(user_buf)] = 0;

			// Cliens want to add new bid
			if(user_buf[0] == 'b' || user_buf[0] == 'B'){
				
				index = 0;
				for(; index < sizeof(user_buf); index ++){
					if(user_buf[index] == ' '){
						break;
					}
				}
				index++;

				my_face = atoi((user_buf + index));

				for(; index < sizeof(user_buf); index ++){
					if(user_buf[index] == ' '){
						break;
					}
				}
				index++;

				my_quan = atoi((user_buf + index));

				game_msg.client_ID = my_ID.client_ID;
				game_msg.bid_face = my_face;
				game_msg.bid_quantity = my_quan;

				//TODO csak nagyobb lehet a tét
				printf("Client: New bid: face %d quantity %d\n", my_face, my_quan);

			} else if(user_buf[0] == 'c' || user_buf[0] == 'C'){

				game_msg.challenge == true;

			} else {
				//TODO ha lesz vmi mutexes/signalos móka itt, akkor a rekurzióval baj lesz
				printf("Client: Hiba: wrong user input. Try again \n");
				process_client_message(phase, msg, msglen);
				break;

			}
			
			game_msg.msgID = NEW_BID;
	
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
