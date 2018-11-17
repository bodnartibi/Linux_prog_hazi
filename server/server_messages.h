#ifndef SERVER_MESSAGES_H
#define SERVER_MESSAGES_H

#include "messages.h"

struct server_newroll_msg 			act_roll;
struct server_prop_bid_msg			prop_act_bid;
struct who_won_msg							won;
struct your_ID									client_ID;
struct client_reg_msg						client_reg;
struct client_ready							client_red;
struct client_game_msg					client_game;
struct info_msg									info;
struct discon_msg								discon_msg;

char buf[256];

int process_server_message(void* msg, int msglen, int dices[][MAX_DICE_NUM]);
void broadcast_disconnected(int disclientID);

#endif
