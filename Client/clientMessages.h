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
    QString my_name;

public slots:
    void process_client_message(void*, int);

signals:
    void send_msg(void* msg, int msglen);

private:
    struct server_prop_client_msg 	clients;
    struct server_newroll_msg		act_roll;
    struct server_prop_bid_msg		act_bid;
    struct who_won_msg				won;
    struct your_ID					my_ID;
    struct client_reg_msg			reg;
    struct client_game_msg			game_msg;
    struct client_ready				ready_msg;
    struct info_msg					info;
};


#endif
