#ifndef SERVER_MESSAGES_H
#define SERVER_MESSAGES_H

#include "messages.h"
#include "server_gamelogic.h"

struct server_prop_client_msg* 	clients;
struct server_newroll_msg* 			act_roll;
struct server_prop_bid_msg*			act_bid;
struct who_won_msg*							won;
struct your_ID*									client_ID;

int process_server_message(int phase, void* msg, int msglen, int dices[][MAX_DICE_NUM]);

#endif
