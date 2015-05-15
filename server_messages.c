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

void end_game(int who_won_id)
{
	int index;
	int client;
	int res;

	won.msgID = WHO_WON;
	
	for(index = 0; index < MAX_CLIENT_NUM; index++){
		client = clients_connfd[index];
		if(client <= 0){
			continue;
		}

		strcpy(client_names[who_won_id],won.name);

		if(client == who_won_id){
			won.is_it_you = 1;
		} else {
			won.is_it_you = 0;
		}

		res = send(client, (void*)&won, sizeof(won), 0);

		if(res < 0){
			fprintf(stderr,"Hiba: Server: send won, index %d client ID %d %s\n", index, client, strerror(errno));
		}		
	}

	for(index = 0; index < MAX_CLIENT_NUM; index++)
	{
		clients_ready[index] = FALSE;
	}

}

void broadcast_disconnected(int disclientID)
{
	int index;
	int client;
	int res;

	discon_msg.msgID = DISCONNECT;
	strcpy(client_names[disclientID],discon_msg.name);

	for(index = 0; index < MAX_CLIENT_NUM; index++){
		client = clients_connfd[index];
		if(client <= 0 || client == disclientID){
			continue;
		}

		res = send(client, (void*)&discon_msg, sizeof(discon_msg), 0);
		
		if(res < 0){
			fprintf(stderr,"Hiba: Server: send disconnected, index %d client ID %d %s\n", index, client, strerror(errno));
		}	
	}
}

void broadcast_info(char* msg){
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
		
		if(res < 0){
			fprintf(stderr,"Hiba: Server: send info, index %d client ID %d %s\n", index, client, strerror(errno));
		}	
	}
}

void new_game(int dices[][MAX_DICE_NUM]){
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

void next_round(){
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
			
			sprintf(buf,"This is %s's round\n",client_names[index]);		
			broadcast_info(buf);
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

int is_every_client_ready(int dices[][MAX_DICE_NUM]){
	int i, res;

	for(i = 0; i < MAX_CLIENT_NUM; i++){
		if((clients_connfd[i] > -1) && (clients_ready[i] != TRUE))
			return FALSE;
	}
	init_dices(dices);


	for(i = 0; i < MAX_CLIENT_NUM; i++){
		if(clients_connfd[i] > -1){
			res = add_client_to_dices(i,dices);
			if(res < 0){
				fprintf(stderr,"Server: Hiba: add_client_to_dices. \n");
			}
		}
	}
	return TRUE;
}

// return:
//  0: nothing special
//  1: clients ready
// -1: error

int process_server_message(void* msg, int msglen, int dices[][MAX_DICE_NUM]) {
	int msg_ID ;
	int	client_ID;
	msg_ID = *(int*)msg;
	client_ID = *(int*)(msg + 1);
	int ret = 0;


	printf("Server: Process message: %d\n", msg_ID);

	switch(msg_ID){

		case REG_CLIENT:
			client_reg = *(struct client_reg_msg*)msg;
			strcpy(client_names[client_reg.client_ID], client_reg.name);
			printf("Server: Client registered name: %s\n", client_names[client_reg.client_ID]);
			
			sprintf(buf,"New player: %s\n",client_names[client_reg.client_ID]);		
			broadcast_info(buf);
	
			break;


		case CLIENT_READY:
			client_red = *(struct client_ready*)msg;
			printf("Server: Client ready id: %d\n", client_red.client_ID);

					if(client_red.ready == TRUE){
						printf("Server: Client ready: ID %d\n", client_red.client_ID);

						sprintf(buf,"%s is ready to play\n",client_names[client_red.client_ID]);		
						broadcast_info(buf);

						clients_ready[client_red.client_ID] = TRUE;

						// mindenki kész?
						if(is_every_client_ready(dices) == TRUE){
							printf("Server: Every client is ready\n");

							sprintf(buf,"Everybody is ready to play\n");		
							broadcast_info(buf);

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
				sprintf(buf,"Challange! %s lost a dice \n",client_names[client_game.client_ID]);	
			} else {
				remove_client_dices(bid_client, 1, dices);
				sprintf(buf,"Challange! %s lost a dice \n",client_names[bid_client]);	
			}

			broadcast_info(buf);

			ret = is_this_end_of_game(dices);
			if(ret > 0)
			{
				printf("Server: Game over\n");
				end_game(ret);
				break;
			}	

			new_game(dices);
			next_round();
			break;
		
		case NEW_BID:
			client_game = *(struct client_game_msg*)msg;
			act_face = client_game.bid_face;
			act_quan = client_game.bid_quantity;
			bid_client = client_game.client_ID;
	
			sprintf(buf,"%s has added a new bid: quantity %d face %d\n",client_names[bid_client],act_quan,act_face);		
			broadcast_info(buf);

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



