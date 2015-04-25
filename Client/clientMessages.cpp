
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

//#include "client.h"
#include <clientMessages.h>

struct server_prop_client_msg 	clients;
struct server_newroll_msg				act_roll;
struct server_prop_bid_msg			act_bid;
struct who_won_msg							won;
struct your_ID									my_ID;
struct client_reg_msg						reg;
struct client_game_msg					game_msg;
struct client_ready							ready_msg;
struct info_msg									info;


void ClientMessages::set_name(const char* name)
{
    ready_msg.msgID = CLIENT_READY;
    ready_msg.client_ID = my_ID.client_ID;
    ready_msg.ready = true;


    emit send_msg(&ready_msg, sizeof(ready_msg));
    return;
}

void ClientMessages::set_new_bid(int quan, int face)
{
    game_msg.msgID = NEW_BID;
    game_msg.client_ID = my_ID.client_ID;
    game_msg.bid_face = face;
    game_msg.bid_quantity = quan;

    send_msg((void*)&game_msg, sizeof(game_msg));

    return;
}

void ClientMessages::challenge()
{
    game_msg.msgID = CHALLENGE;
    game_msg.challenge == true;

    send_msg((void*)&game_msg, sizeof(game_msg));
    return;
}

void ClientMessages::process_client_message(void* msg, int msglen)
{
    int msg_ID ;
    msg_ID = *(int*)msg;
    int res;

    switch(msg_ID){

        case YOUR_ID:
            my_ID = *(struct your_ID*)msg;
            reg.msgID = REG_CLIENT;
            reg.client_ID = my_ID.client_ID;
            strcpy(&reg.name[0], my_name.toStdString().c_str());
            emit send_msg(&reg, sizeof(reg));

            if(msglen > sizeof(your_ID))
                process_client_message((msg+sizeof(your_ID)), msglen - sizeof(your_ID));

            break;

        case PROP_BID:
            act_bid = *(struct server_prop_bid_msg*)msg;
            emit new_bid(act_bid.bid_quantity, act_bid.bid_face);

            if(act_bid.your_turn == true){
                emit this_is_your_turn();
            }

            if(msglen > sizeof(PROP_BID))
                process_client_message((msg+sizeof(PROP_BID)), msglen - sizeof(PROP_BID));
            break;

        case NEW_DICE_ROLL:
            act_roll = *(struct server_newroll_msg*)msg;
            emit new_dices(&act_roll.dices[0]);

            if(msglen > sizeof(NEW_DICE_ROLL))
                process_client_message((msg+sizeof(NEW_DICE_ROLL)), msglen - sizeof(NEW_DICE_ROLL));
            break;

        case INFO:

            info = *(struct info_msg*)msg;
            printf("\n====== \n%s",info.msg);
            if(msglen > sizeof(INFO))
                process_client_message((msg+sizeof(INFO)), msglen - sizeof(INFO));
            break;

        default:
            fprintf(stderr,"Hiba: Client: invalid messagetype msgID: %d\n", msg_ID);
            break;
    }
    printf("Client: get message ID %d\n",msg_ID);
    return;
//	new_dices(dices);
}
