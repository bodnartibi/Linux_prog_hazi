#include "server_gamelogic.h"
#include "messages.h"

// ----------
// Game logic
// ----------

// ----------
// New dices
// osszunk új kockákat
// ahol negatív szerepel, oda nem osztunk újat

void new_dices(int dices_array[][MAX_DICE_NUM]){
	int client;
	int dice;
	for(client = 0; client < MAX_CLIENT_NUM; client ++) {
		for(dice = 0; dice < MAX_DICE_NUM; dice ++){
			if (dices_array[client][dice] < 0 ){
				continue;
			}
			dices_array[client][dice] = rand() % 6 + 1;
		}	
	}
}

// ------------------------
// Add client to dice array
// Megkeressük az első üres helyet

int add_client_to_dices(int dices_array[][MAX_DICE_NUM]){
	int client;
	int dice;
	int index = -1;
	for(client = 0; client < MAX_CLIENT_NUM; client ++) {
		if (dices_array[client][0] > -1){
			continue;
		}
		for(dice = 0; dice < MAX_DICE_NUM; dice ++){
			dices_array[client][dice] = 0;
		}
		index = client;
		break;
	}
	return index;
}

// ----------------
// Challenge result
// ellenőrizzük, hogy a bid helytáll-e

int check_challenge(int face, int quantity,int dices_array[][MAX_DICE_NUM]){
	int client;
	int dice;
	int sum = 0;
	for(client = 0; client < MAX_CLIENT_NUM; client ++) {
		for(dice = 0; dice < MAX_DICE_NUM; dice ++){
			if(dices_array[client][dice] < 0){
				continue;
			}
			if (dices_array[client][dice] == face || dices_array[client][dice] == 1) {
				sum ++;
			}
		}
	}
	return (sum >= quantity);
}

