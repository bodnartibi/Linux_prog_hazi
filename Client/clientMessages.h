#ifndef CLIENTMESSAGES_H
#define CLIENTMESSAGES_H

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <QString>
#include "../messages.h"

#include <QObject>

class ClientMessages : public QObject
{
    Q_OBJECT

public:
    server_prop_bid_msg getBid();

public slots:
    void process_client_message(void*, int);
    void set_name(const char* name);
    void set_new_bid(int quan, int face);
    void challenge();
    void client_is_ready();


signals:
    void send_msg(void* msg, int msglen);
    void this_is_your_turn();
    void new_bid(int quan, int face);
    void new_dices(int* dices);
    void new_info_msg(const char* name);
    void sy_disconnected(const char* name);
    void sy_won(const char* name, bool is_it_you);

private:
    struct server_newroll_msg		act_roll;
    struct server_prop_bid_msg		act_bid;
    struct who_won_msg				won;
    struct your_ID					my_ID;
    struct client_reg_msg			reg;
    struct client_game_msg			game_msg;
    struct client_ready				ready_msg;
    struct info_msg					info;
    struct discon_msg				discon_msg;

};


#endif
