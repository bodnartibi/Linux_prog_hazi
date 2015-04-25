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
#include "server_gamelogic.h"
#include "server.h"

// melyik jatekos jon epp
int act_client;

int act_face;
int act_quan;
int bid_client;


int broadcast_info(char* msg){
	int index;
	int client;
	int res = 0;

	info.msgID = INFO;
	strcpy(info.msg, msg);

	for(index = 0; index < MAX_CLIENT_NUM; index++){
		client = clients_connfd[index];
		if(client <= 0){
			continue;
		}

		res = send(client, (void*)&info, sizeof(info), 0);
		
		//TODO ha több üzenetet nem fogadot tmgé akliens akkor azok ugyanabba a bufferbe írodnak, ezt ott kezelni kell
		// erre hack itt a sleep
		sleep(1);
		if(res < 0){
			fprintf(stderr,"Hiba: Server: send info, index %d client ID %d %s\n", index, client, strerror(errno));
		}	
	}
}

int new_game(int dices[][MAX_DICE_NUM]){
	int client, index, j, res;

	printf("Server: New game\n");
	broadcast_info("New game\n");
	act_face = 0;
	act_quan = 0;
	act_client = 0;
	new_dices(dices);

	act_roll.msgID = NEW_DICE_ROLL;
	
	for(index = 0; index < MAX_CLIENT_NUM; index++){
		client = clients_connfd[index];
		if(client <= 0){
			continue;
		}
		for(j = 0; j < MAX_DICE_NUM; j++){
			act_roll.dices[j] = dices[index][j];
		}

		res = send(client, (void*)&act_roll, sizeof(act_roll),0);
		
		if(res < 0){
			fprintf(stderr,"Hiba: Server: send message in new_game, index %d client ID %d %s\n", index, client, strerror(errno));
		}		

	}
}

// kovetkezo kor
// PROP_BID msg kuldese

int next_round(){
	int index;
	int res;
	int client;

	prop_act_bid.msgID  = PROP_BID;
	prop_act_bid.bid_face = act_face;
	prop_act_bid.bid_quantity = act_quan;

	if(act_client > clients_num - 1){
		act_client = 0;
	}

	broadcast_info("New round\n");
	printf("Server: New round\n");
	for(index = 0; index < MAX_CLIENT_NUM; index++){
		client = clients_connfd[index];
		if(client <= 0){
			continue;
		}

		if(index == act_client){
			prop_act_bid.your_turn = TRUE;
		} else {
			prop_act_bid.your_turn = FALSE;
		}

		printf("Server: New round msg sent to client: index %d act client: %d\n",index, act_client);
		res = send(client, (void*)&prop_act_bid, sizeof(prop_act_bid),0);
		
		if(res < 0){
			fprintf(stderr,"Hiba: Server: send message in next round, index %d client ID %d %s\n", index, client, strerror(errno));
		}		

	}

	act_client++;
}

int is_every_client_ready(){
	int i, j;
	int ready = TRUE;

	for(i = 0; i < MAX_CLIENT_NUM; i++){
		if((clients_connfd[i] > -1) && (clients_ready[i] != TRUE)){
			ready = FALSE;
			break;
		}
	}
	return ready;
}

// return:
//  0: nothing special
//  1: clients ready
// -1: error

int process_server_message(int phase, void* msg, int msglen, int dices[][MAX_DICE_NUM]) {
	int msg_ID ;
	int	client_ID;
	int index, i, j;
	msg_ID = *(int*)msg;
	client_ID = *(int*)(msg + 1);
	int ret = 0;
	char buf[256];

	// ------------------------------
	// check if ID matches with phase
	// TODO ez még gány
//	if( ((msg_ID >= 0x10) && (phase != 0)) || 
//			((msg_ID < 0x10 || msg_ID >= 0x30) && (phase != 1)) || 
//			((msg_ID <= 0x30) && (phase != 2))
//		){
//		fprintf(stderr,"Hiba: Server: invalid message Phase %d msgID: %d . \n", phase, msg_ID);
//		return -1;
//	}
	
	printf("Server: Process message: %d\n", msg_ID);

	switch(msg_ID){

		case REG_CLIENT:
			client_reg = *(struct client_reg_msg*)msg;
			strcpy(client_names[client_reg.client_ID], client_reg.name);
			printf("Server: Client registered name: %s\n", client_names[client_reg.client_ID]);
			break;


		case CLIENT_READY:
			client_red = *(struct client_ready*)msg;
			printf("Server: Client ready id: %d\n", client_red.client_ID);

					if(client_red.ready == TRUE){
						printf("Server: Client ready: ID %d\n", client_red.client_ID);
						clients_ready[client_red.client_ID] = TRUE;

						// mindenki kész?
						if(is_every_client_ready() == TRUE){
							printf("Server: Every client is ready\n");
							ret = 1;
							new_game(dices);
							next_round();
						}

						break;
					}
					else if(client_red.ready == FALSE){
						printf("Server: Client doesn't ready: ID %d\n", client_red.client_ID);
						clients_ready[client_red.client_ID] = FALSE;
						break;
					}
					else{
						fprintf(stderr,"Hiba: Server: invalid ready state: %d", client_red.ready);
						break;
					}


			break;

		case CHALLENGE:
			client_game = *(struct client_game_msg*)msg;
			ret = check_challenge(act_face, act_quan, dices);
			if(ret){
				remove_client_dices(client_game.client_ID, 1, dices);
				sprintf(buf,"Challange! Client %d lost a dice \n",client_game.client_ID);	
			} else {
				remove_client_dices(bid_client, 1, dices);
				sprintf(buf,"Challange! Client %d lost a dice \n",bid_client);	
			}
			
			broadcast_info(buf);
			new_game(dices);
			next_round();
			break;
		
		case NEW_BID:
			client_game = *(struct client_game_msg*)msg;
			act_face = client_game.bid_face;
			act_quan = client_game.bid_quantity;
			bid_client = client_game.client_ID;
			next_round();
			break;

		default:
			fprintf(stderr,"Hiba: Server: unknown message: %d", msg_ID);
			ret = -1;
			break;
	}
	
	printf("Server: get message ID %d client %d\n",msg_ID, ((struct client_gen_msg*)msg)->client_ID);
	return ret;
}



