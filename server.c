#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include "messages.h"

// TODO hack: tudni kellene mekkora a legnagyobb üzenet
#define READ_SIZE 1024
#define ez


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



// az összes kocka
// a kliens nem érvényes kockáinál -1
// a kliens nem létező kockáinál -2
// (nemlétező kliensnek -2 a kockái)
int dices[MAX_CLIENT_NUM][MAX_DICE_NUM];
// könynebb kezelhetőségért a kliensek száma
int clients_num;

int dices_per_client[MAX_CLIENT_NUM];

int init_communication(){
	int rcv_fd;
  rcv_fd = open("to_server", O_RDONLY);
  if(rcv_fd < 0)
  {
    perror("open");
    return EXIT_FAILURE;
  }

	return rcv_fd;
}

int get_message(int fd, char* p){
	int len;	
	//char buf[READ_SIZE];
	int ID;
	len = read(fd, p, sizeof(READ_SIZE));
  if(len < 0)
  {
    perror("read");
    return -1;
  }
	p[len] = '\0';
	//p = buf;
	//printf("az2 %s\n", buf);
	printf("az1 %s\n", p);

	return len;
}

int main(int argc, char* argv[]){
	int res, client, dice;
	int fd;
	char * msg;
	int ID;
	msg = malloc(sizeof(READ_SIZE));
	fd = init_communication();
	//res = get_message(fd, msg);
	//printf("ez %s\n", msg);

	for(client = 0; client < MAX_CLIENT_NUM; client ++)
	for(dice = 0; dice < MAX_DICE_NUM; dice ++)
	dices[client][dice] = -2;

	add_client_to_dices(dices);
	add_client_to_dices(dices);
	
	new_dices(dices);

	for(client = 0; client < MAX_CLIENT_NUM; client ++)
	for(dice = 0; dice < MAX_DICE_NUM; dice ++)
	printf("dice %d\n", dices[client][dice]);

	return 0;
}





