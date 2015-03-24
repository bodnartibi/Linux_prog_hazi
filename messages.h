#ifndef MESSAGES_H
#define MESSAGES_H


#define MAX_CLIENT_NUM 		5
#define MAX_NAME_LENGTH 	16
#define MAX_DICE_NUM 			5

// -------------------------
// Client to server messages
// -------------------------

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
#define PROP_CLIENT				0x40

// Game phase
#define NEW_DICE_ROLL			0x50
#define YOUR_TURN 				0x51
#define PROP_BID					0x52
#define WHO_WON						0x53

// End phase



// ---------------------
// Structs
// ---------------------

// Generic message

struct client_gen_msg {
	int ID;
	void* content;
};

// ---------------------
// Client to server

// One client sends
struct client_reg_msg {
	int ID;
	char name[MAX_NAME_LENGTH];
	int ready;
};

// One client sends to server
struct client_game_msg{
	int ID;
	char name[MAX_NAME_LENGTH];
	int challenge;
	int bid_face;
	int bid_quantity;	
};


// ---------------------
// Server to client


// To all clients
struct server_prop_client_msg{
	int ID;
	char clients[MAX_CLIENT_NUM][MAX_NAME_LENGTH];
};

// To one
struct server_newroll_msg{
	int ID;
	int dices[MAX_DICE_NUM];
};

// Your turn message: only the msg ID which we need

// To one
struct server_prop_bid_msg{
	int ID;
	int bid_face;
	int bid_quantity;
};

struct who_won_msg{
	int ID;
	char name[MAX_NAME_LENGTH];
};

#endif

