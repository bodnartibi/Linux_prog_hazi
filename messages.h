#ifndef MESSAGES_H
#define MESSAGES_H

#define TRUE 1
#define FALSE 0
#define true TRUE
#define false FALSE


#define MAX_CLIENT_NUM 		2
#define MAX_NAME_LENGTH 	16
#define MAX_DICE_NUM 			5

#define SOCKET_SIZE 1024
#define INFO_SIZE 256

// ------
// STATES
// ------

#define REG_PHASE 0
#define GAME_PHASE 1
#define END_PHASE 2

// TODO kezdőértéknek is meg lehet adni az msgID-t, felesleges máshol megadni


// -------------------------
// Client to server messages
// -------------------------

// Intervals
#define REG_PHASE_MSG_INTERVAL 0x00
#define GAME_PHASE_MSG_INTERVAL 0x10
#define END_PHASE_MSG_INTERVAL 0x20

// Register phase

#define REG_CLIENT				0x00
#define CLIENT_READY			0x01

// Game phase


#define CHALLENGE					0x12
#define NEW_BID						0x13

// End phase


// -------------------------
// Server to client messages
// -------------------------

// Register phase
#define PROP_CLIENT				0x00
#define YOUR_ID						0x01

// Game phase
#define NEW_DICE_ROLL			0x10
#define PROP_BID					0x12
#define WHO_WON						0x13

// End phase


#define INFO							0x40



// ---------------------
// Structs
// ---------------------

// Generic message

struct client_gen_msg {
	int msgID;
	int client_ID;
	void* content;
};

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
	char name[MAX_NAME_LENGTH];
	int challenge;
	int bid_face;
	int bid_quantity;	
};


// ---------------------
// Server to client


// To all clients
struct server_prop_client_msg{
	int msgID;
	char clients[MAX_CLIENT_NUM][MAX_NAME_LENGTH];
};

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
#endif

