#ifndef MESSAGES_H
#define MESSAGES_H


#define MAX_CLIENT_NUM 		1
#define MAX_NAME_LENGTH 	16
#define MAX_DICE_NUM 			5

#define SOCKET_SIZE 1024

// ------
// STATES
// ------

#define REG_PHASE 0
#define GAME_PHASE 1
#define END_PHASE 2


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
#define YOUR_TURN 				0x11
#define PROP_BID					0x12
#define WHO_WON						0x13

// End phase



// ---------------------
// Structs
// ---------------------

// Generic message

struct client_gen_msg {
	int msgID;
	void* content;
};

// ---------------------
// Client to server

// One client sends
struct client_reg_msg {
	int msgID;
	int client_ID;
	char name[MAX_NAME_LENGTH];
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

// Your turn message: only the msg ID which we need

// To one
struct server_prop_bid_msg{
	int msgID;
	int bid_face;
	int bid_quantity;
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
#endif

