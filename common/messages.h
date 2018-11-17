#ifndef MESSAGES_H
#define MESSAGES_H

#define TRUE 1
#define FALSE 0

#define MAX_CLIENT_NUM 		5
#define MAX_NAME_LENGTH 	16
#define MAX_DICE_NUM 			5

#define SOCKET_SIZE 1024
#define INFO_SIZE 256

// -------------------------
// Client to server messages
// -------------------------

#define REG_CLIENT				0x00
#define CLIENT_READY			0x01
#define CHALLENGE					0x12
#define NEW_BID						0x13

// -------------------------
// Server to client messages
// -------------------------

#define YOUR_ID						0x01
#define NEW_DICE_ROLL			0x10
#define PROP_BID					0x12
#define WHO_WON						0x13
#define DISCONNECT              0x41
#define INFO							0x40

// ---------------------
// Structs
// ---------------------

// ---------------------
// Client to server

// One client sends
struct client_reg_msg{
	int msgID;
	int client_ID;
	char name[MAX_NAME_LENGTH];
};

struct client_ready{
	int msgID;
	int client_ID;
	int ready;
};

// One client sends to server
struct client_game_msg{
	int msgID;
	int client_ID;
	int challenge;
	int bid_face;
	int bid_quantity;	
};


// ---------------------
// Server to client

// To one about dices
struct server_newroll_msg{
	int msgID;
	int dices[MAX_DICE_NUM];
};

// Your turn message
// and prop bid
// your_turn if it is your turn
// To one
struct server_prop_bid_msg{
	int msgID;
	int bid_face;
	int bid_quantity;
	int your_turn;
};

struct who_won_msg{
	int msgID;
  int is_it_you;
	char name[MAX_NAME_LENGTH];
};

// Your ID
struct your_ID{
	int msgID;
	int client_ID;
};

struct info_msg{
	int msgID;
	char msg[INFO_SIZE];
};

struct discon_msg{
    int msgID;
    char name[MAX_NAME_LENGTH];
};
#endif

