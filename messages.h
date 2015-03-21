// -------------------------
// Client to server messages
// -------------------------

// Register phase
#define REG_CLIENT	0x00
#define CLIENT_READY	0x01

// Game phase
#define CHALLENGE	0x12
#define NEW_BID		0x13

// End phase

// -------------------------
// Server to client messages
// -------------------------

// Register phase
#define PROP_CLIENT	0x40

// Game phase
#define NEW_DICE_ROLL	0x50
#define WHO_IS_THE_NEXT 0x51
#define PROP_BID	0x52
#define WHO_WON		0x53

//End phase
