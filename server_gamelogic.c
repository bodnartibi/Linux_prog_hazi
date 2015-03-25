#include "server_gamelogic.h"
#include "messages.h"

// ----------
// Game logic
// ----------

// Dices:
// -2: invalid dice
// -1: removed dice (client had a dice)
//  0: valid dice

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

int check_challenge(int face, int quantity, int dices_array[][MAX_DICE_NUM]){
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

// ----------------
// Kliens kockájának visszavonása
// kilesn IDja, visszavonandó kockák száma

int remove_client_dices(int client_id, int dices_num, int dices_array[][MAX_DICE_NUM]){
	int dice;
	int found = -1;
	int num;
	if(client_id >= MAX_CLIENT_NUM){
		return -1;
	}
	for(num = 0; num < dices_num; num ++){
		for(dice = MAX_DICE_NUM; dice > -1; dice --){
			if(dices_array[client_id][dice] > -1){
				dices_array[client_id][dice] = -1;
				found = 0;
				break;
			}
		}
	}
	return found;
}








