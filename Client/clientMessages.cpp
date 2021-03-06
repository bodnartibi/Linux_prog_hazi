
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

//#include "client.h"
#include <clientMessages.h>

void ClientMessages::client_is_ready()
{
    ready_msg.msgID = CLIENT_READY;
    ready_msg.client_ID = my_ID.client_ID;
    ready_msg.ready = true;

    emit send_msg(&ready_msg, sizeof(ready_msg));
}

void ClientMessages::set_name(const char* name)
{ 
    int len;
    len = strlen(name);
    if(len > MAX_NAME_LENGTH)
        len = MAX_NAME_LENGTH;
    reg.msgID = REG_CLIENT;
    reg.client_ID = my_ID.client_ID;
    memcpy(&reg.name[0], name, len);
    reg.name[MAX_NAME_LENGTH - 1] = 0;
    emit send_msg(&reg, sizeof(reg));
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
    char buff[256];
    int i;

    switch(msg_ID){

        case YOUR_ID:
            my_ID = *(struct your_ID*)msg;
            fprintf(stderr,"Client: get message YOUR_ID %d\n", my_ID.client_ID);
            if(msglen > sizeof(your_ID))
                process_client_message((msg+sizeof(your_ID)), msglen - sizeof(your_ID));

            break;

        case PROP_BID:
            fprintf(stderr,"Client: get message PROP_BID \n");
            act_bid = *(struct server_prop_bid_msg*)msg;
            emit new_bid(act_bid.bid_quantity, act_bid.bid_face);

            if(act_bid.your_turn == true){
                emit this_is_your_turn();
            }

            if(msglen > sizeof(act_bid))
                process_client_message((msg+sizeof(act_bid)), msglen - sizeof(act_bid));
            break;

        case NEW_DICE_ROLL:
            fprintf(stderr,"Client: get message NEW_DICE_ROLL \n");
            act_roll = *(struct server_newroll_msg*)msg;
            emit new_dices(&act_roll.dices[0]);

            sprintf(buff,"You have new dices: ");
            for(i = 0; i < MAX_DICE_NUM; i++)
            {
                if(act_roll.dices[i] > 0)
                    sprintf(buff + strlen(buff), "%d ",act_roll.dices[i]);
            }

            emit new_info_msg(buff);

            if(msglen > sizeof(act_roll))
                process_client_message((msg+sizeof(act_roll)), msglen - sizeof(act_roll));
            break;

        case INFO:
            fprintf(stderr,"Client: get message INFO \n");
            info = *(struct info_msg*)msg;
            printf("\n====== \n%s",info.msg);
            emit new_info_msg(info.msg);
            if(msglen > sizeof(info))
                process_client_message((msg+sizeof(info)), msglen - sizeof(info));
            break;

        case DISCONNECT:
            fprintf(stderr,"Client: get message DISCONNECT \n");
            discon_msg = *(struct discon_msg*)msg;
            emit sy_disconnected(discon_msg.name);

            if(msglen > sizeof(discon_msg))
                process_client_message((msg+sizeof(discon_msg)), msglen - sizeof(discon_msg));
            break;

        case WHO_WON:

            won = *(struct who_won_msg*)msg;
            emit sy_won(won.name, (bool)won.is_it_you);

            if(msglen > sizeof(won))
                process_client_message((msg+sizeof(won)), msglen - sizeof(won));
            break;

        default:
            fprintf(stderr,"Hiba: Client: invalid messagetype msgID: %d\n", msg_ID);
            break;
    }


    return;
}
