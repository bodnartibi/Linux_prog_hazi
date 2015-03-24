#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include "messages.h"
#include "server_gamelogic.h"

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
	//fd = init_communication();
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





