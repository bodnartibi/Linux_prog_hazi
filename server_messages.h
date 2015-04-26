#ifndef SERVER_MESSAGES_H
#define SERVER_MESSAGES_H

#include "messages.h"

struct server_prop_client_msg 	clients;
struct server_newroll_msg 			act_roll;
struct server_prop_bid_msg			prop_act_bid;
struct who_won_msg							won;
struct your_ID									client_ID;
struct client_reg_msg						client_reg;
struct client_ready							client_red;
struct client_game_msg					client_game;
struct info_msg									info;

char buf[256];

int process_server_message(int phase, void* msg, int msglen, int dices[][MAX_DICE_NUM]);

#endif
