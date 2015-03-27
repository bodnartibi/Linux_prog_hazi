#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "messages.h"
#include "server_gamelogic.h"

// az összes kocka
// a kliens nem érvényes kockáinál -1
// a kliens nem létező kockáinál -2
// (nemlétező kliensnek -2 a kockái)
int dices[MAX_CLIENT_NUM][MAX_DICE_NUM];
// könynebb kezelhetőségért a kliensek száma
int clients_num;


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
	int res, client, dice, face, quan;
	int fd;
	char * msg;
	int ID;
	msg = malloc(sizeof(READ_SIZE));
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr; 

	char sendBuff[1025];
	time_t ticks; 

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000); 

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

	listen(listenfd, 10); 

	while(1)
	{
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

		ticks = time(NULL);
		snprintf(sendBuff, sizeof(sendBuff), "Ez \n");
		write(connfd, sendBuff, strlen(sendBuff)); 

		close(connfd);
		sleep(1);
	}

	//fd = init_communication();
	//res = get_message(fd, msg);
	//printf("ez %s\n", msg);

	//printf("hamis %d\n", (1 > 2));
  //printf("igaz %d\n", (2 > 1));
/*

*/


	return 0;
}





