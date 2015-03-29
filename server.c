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
#include "server_messages.h"
#include "server.h"

int listenfd;

// ------------------------
// Send messages to clients
// ------------------------

void* send_to_clients(){
/*
	int index;
	char sendBuff[SOCKET_SIZE];
	int clients_num = 0;

	printf("Server: Send thread started \n");
	while(1){
		pthread_mutex_lock(&clients_mutex);
		printf("Server: Send thread wait \n");
		pthread_cond_wait (&clients_cond,&clients_mutex);
		printf("Server: Send thread wait return \n");
	
		for(index = 0, clients_num = 0; index < MAX_CLIENT_NUM; index++){
			if(clients_connfd[index] > -1){
				clients_num ++;
			}
		}

		// send ID to client
		
		write(clients_connfd[index], sendBuff, strlen(sendBuff));
		
//		snprintf(sendBuff, sizeof(sendBuff), "Kliensek szama: %d \n",clients_num);

		for(index = 0, clients_num = 0; index < MAX_CLIENT_NUM; index++){
			if(clients_connfd[index] > -1){
				//write(clients_connfd[index], sendBuff, strlen(sendBuff)); 
			}
		}
		pthread_mutex_unlock(&clients_mutex);
	}	
	printf("Server: Send thread return \n");
*/
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

	pthread_t accept_thread;
	pthread_t send_thread;

	if(argc != 2)
	{
		printf("\n Usage: %s <ip of server> \n",argv[0]);
		return 1;
	} 

	pthread_mutex_init(&clients_mutex,NULL);
	pthread_cond_init(&clients_cond,NULL);
	pthread_mutex_init(&state_mutex,NULL);
	pthread_cond_init(&state_cond,NULL);
	
	state = REG_PHASE;

	//
	// socket létrehozása
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	if(listenfd < 0){
		fprintf(stderr,"Server:Hiba: open socket %s. \n",strerror(errno));;
		exit(EXIT_FAILURE);
	}
	
	printf("Socket created. %d \n", listenfd);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff)); 

	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = htonl("Server: 127.0.0.5");
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
	{
		fprintf(stderr,"Server: Hiba: inet_pton %s. \n",strerror(errno));
		return 1;
	} 
	serv_addr.sin_port = htons(5000); 

	//
	// bind
	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
		fprintf(stderr,"Server: Hiba: bind %s. \n",strerror(errno));;
		exit(EXIT_FAILURE);
	}

	//
	// listen
	if(listen(listenfd, MAX_CLIENT_NUM) < 0){
		fprintf(stderr,"Server: Hiba: listen. \n");
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&accept_thread, NULL, accept_clients, (void*)&listenfd))
	{
		fprintf(stderr,"Server: Hiba: thread inditas, accept_thread %s. \n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&send_thread, NULL, send_to_clients, (void*)&listenfd))
	{
		fprintf(stderr,"Server: Hiba: thread inditas, send_thread %s. \n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	// mindenki küldött readyt
	pthread_mutex_lock(&state_mutex);
	pthread_cond_wait (&state_cond,&state_mutex);
	pthread_cancel(accept_thread);
	state = GAME_PHASE;
	pthread_mutex_unlock(&state_mutex);



	
	pthread_join(accept_thread, NULL);

	// végetért a regisztrációs fázis


	pthread_join(send_thread, NULL);
	
	

	return 0;
}





