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
#include <pthread.h>

#include "messages.h"
#include "server_gamelogic.h"

// az összes kocka
// a kliens nem érvényes kockáinál -1
// a kliens nem létező kockáinál -2
// (nemlétező kliensnek -2 a kockái)
int dices[MAX_CLIENT_NUM][MAX_DICE_NUM];
// könynebb kezelhetőségért a kliensek száma
int clients_num;

// kliensek
// mutex: client_mutex
// condition: clients_con
int clients_connfd[MAX_CLIENT_NUM];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clients_cond = PTHREAD_COND_INITIALIZER;

int listenfd;

void* send_to_clients(){
	int index;
	char sendBuff[1025];
	int clients_num = 0;

	printf("Send thread started \n");
	while(1){
		pthread_mutex_lock(&clients_mutex);
		printf("Send thread wait \n");
		pthread_cond_wait (&clients_cond,&clients_mutex);
		printf("Send thread wait return \n");
	
		for(index = 0, clients_num = 0; index < MAX_CLIENT_NUM; index++){
			if(clients_connfd[index] > -1){
				clients_num ++;
			}
		}
		snprintf(sendBuff, sizeof(sendBuff), "Kliensek szama: %d \n",clients_num);
	
		for(index = 0, clients_num = 0; index < MAX_CLIENT_NUM; index++){
			if(clients_connfd[index] > -1){
				write(clients_connfd[index], sendBuff, strlen(sendBuff)); 
			}
		}
		pthread_mutex_unlock(&clients_mutex);
	}	
	printf("Send thread return \n");
}

// TODO listenfd-t argumentumkent
void* accept_clients(){
	int index;
	int found;
	int connfd;
	struct sockaddr client_addr;
	socklen_t client_addr_len;

	printf("Accept thread started \n");
	while(1){

		// TODO blockolora
		//while(connfd < 0){
		printf("Waiting for clients socket %d\n",listenfd);
		if(	(connfd = accept(listenfd, &client_addr, &client_addr_len)) < 0){
			fprintf(stderr,"Hiba: accept %s. \n",strerror(errno));
		} 
		printf("Accept client: %d \n", connfd);
		//}

		pthread_mutex_lock(&clients_mutex);

		found = 0;
		for(index = 0; index < MAX_CLIENT_NUM; index++){
			if(clients_connfd[index] == connfd){
				break;
			}
			if(clients_connfd[index] < 0){
				found = 1;
				clients_connfd[index] = connfd;
				printf("New client added. index: %d fd: %d\n",index, connfd);
				break;
			}
		}
		if(found){
			pthread_cond_signal (&clients_cond);
		}
		pthread_mutex_unlock(&clients_mutex);
		sleep(1);

		//if(found == 0){
		//	printf("Accept thread return \n");
		//	return;
		//}
	}
	printf("Accept thread return \n");
}

// TODO mi van a conn closeokkal? hol?

// close(connfd);
//


int main(int argc, char* argv[]){
	int res, client, dice, face, quan;
	int fd;
	char * msg;
	int ID;
	int index;
	msg = malloc(sizeof(READ_SIZE));
	struct sockaddr_in serv_addr;


	char sendBuff[1025];
	time_t ticks; 

	pthread_t accept_thread;
	pthread_t send_thread;

	if(argc != 2)
	{
		printf("\n Usage: %s <ip of server> \n",argv[0]);
		return 1;
	} 

	// klienstömb inicializálás
	for(index = 0; index < MAX_CLIENT_NUM; index++)
		clients_connfd[index] = -1;

	//
	// socket létrehozása
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	if(listenfd < 0){
		fprintf(stderr,"Hiba: open socket %s. \n",strerror(errno));;
		exit(EXIT_FAILURE);
	}
	
	printf("Socket created. %d \n", listenfd);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff)); 

	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = htonl("127.0.0.5");
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
	{
		fprintf(stderr,"Hiba: inet_pton %s. \n",strerror(errno));
		return 1;
	} 
	serv_addr.sin_port = htons(5000); 

	//
	// bind
	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
		fprintf(stderr,"Hiba: bind %s. \n",strerror(errno));;
		exit(EXIT_FAILURE);
	}

	//
	// listen
	if(listen(listenfd, MAX_CLIENT_NUM) < 0){
		fprintf(stderr,"Hiba: listen. \n");
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&accept_thread, NULL, accept_clients, NULL))
	{
		fprintf(stderr,"Hiba: thread inditas, accept_thread %s. \n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&send_thread, NULL, send_to_clients, NULL))
	{
		fprintf(stderr,"Hiba: thread inditas, send_thread %s. \n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	pthread_join(accept_thread, NULL);

	return 0;
}





