#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>

#include <sys/select.h>
#include <sys/time.h>

#include <pthread.h>

#include "messages.h"
#include "server_gamelogic.h"
#include "server_messages.h"
#include "server.h"

int listenfd;

int main(int argc, char* argv[]){
	int res, dice, face, quan,i, found;
	int fd;
	int new_socket, client;
	char * msg;
	int ID;
	int index;
	struct sockaddr client_addr;
	char recvBuff[SOCKET_SIZE];
	socklen_t client_addr_len;
	int client_socket[MAX_CLIENT_NUM];

	struct sockaddr_in serv_addr;
	//set of socket descriptors
	fd_set readfds, readfds_copy;
	int max_fd;
	client_addr_len = sizeof(client_addr);

	
	msg = malloc(sizeof(READ_SIZE));

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

	for(index = 0; index < MAX_CLIENT_NUM; index++){
		clients_connfd[index] = -1;
	}

	//
	// socket létrehozása
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	if(listenfd < 0){
		fprintf(stderr,"Server: Hiba: open socket %s. \n",strerror(errno));;
		exit(EXIT_FAILURE);
	}
	
	printf("Socket created. %d \n", listenfd);
	memset(&serv_addr, '0', sizeof(serv_addr));
	//memset(sendBuff, '0', sizeof(sendBuff)); 

	serv_addr.sin_family = AF_INET;
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
	{
		fprintf(stderr,"Server: Hiba: inet_pton %s. \n",strerror(errno));
		return 1;
	} 
	serv_addr.sin_port = htons(5000); 

	//
	// bind
	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
		fprintf(stderr,"Server: Hiba: bind %s. \n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	//
	// listen
	if(listen(listenfd, MAX_CLIENT_NUM) < 0){
		fprintf(stderr,"Server: Hiba: listen. \n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < MAX_CLIENT_NUM; i++){
		client_socket[i] = 0;
	}

	//clear the socket set
	FD_ZERO(&readfds);
  
	//add master socket to set
	FD_SET(listenfd, &readfds);
	max_fd = listenfd;

	printf("Waiting for clients\n");

	while(true){
		
		// másolunk, mert a select módosít
		readfds_copy = readfds;

		printf("Select...\n");		
		res = select(max_fd + 1 , &readfds_copy, NULL , NULL , NULL);
		if(res < 0){
			fprintf(stderr,"Server: Hiba: select %s. \n",strerror(errno));
		}

		// valami történt

		// a listen volt?
		if (FD_ISSET(listenfd, &readfds_copy)){
			printf("Catch connecting\n");
			if ((new_socket = accept(listenfd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0){
				fprintf(stderr,"Server: Hiba: Accept %s. \n",strerror(errno));
				exit(EXIT_FAILURE);
			}
			
			for(index = 0; index < MAX_CLIENT_NUM; index++){
//			if(clients_connfd[index] == connfd){
//				break;
//			}
				if(clients_connfd[index] < 0){
					found = 1;
					clients_connfd[index] = new_socket;

					// adjuk hozzá h a select rá is figyeljen
					FD_SET(new_socket, &readfds);
					if(new_socket > max_fd){
						max_fd = new_socket;
					}
					
					clients_num ++;
					printf("Server: New client added. index: %d fd: %d\n",index, new_socket);	
				}
				else{
					continue;
				}

				// answer
				client_ID.msgID = YOUR_ID;
				client_ID.client_ID = index;
				printf("Server: yourID index: %d\n",index);
				res = write(clients_connfd[index], (void*)&client_ID, sizeof(struct your_ID));
				if(res < 0){
					fprintf(stderr,"Server: Hiba: write %d %s. \n",res,strerror(errno));
				}

				break;
			}
		}
		else{
		// a kliens küldött vmit
			printf("Catch message from clients\n");
			for (i = 0; i < MAX_CLIENT_NUM; i++){
				client = client_socket[i];
				// ha nem ez volt akkor megyünk tovább	
				if (!FD_ISSET(client, &readfds_copy)){
						continue;
				}
				printf("Catch message client: %d\n", i);
				if((res = read(listenfd, recvBuff, sizeof(recvBuff)-1)) > 0){
					recvBuff[res] = 0;
		
					res = process_server_message(state, recvBuff, sizeof(recvBuff),dices);
				}
				else{
					fprintf(stderr,"Server: Hiba: read %d %s. \n",res,strerror(errno));
				}

			}
		}

	}
	

	return 0;
}





