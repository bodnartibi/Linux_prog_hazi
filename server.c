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
	int res, dice, face, quan, found;
	int fd;
	int new_socket, client;
	char * msg;
	int ID;
	int index;
	struct sockaddr client_addr;
	char recvBuff[SOCKET_SIZE];
	socklen_t client_addr_len;

	struct sockaddr_in serv_addr;
	//set of socket descriptors
	fd_set readfds, readfds_copy;
	int max_fd;
	client_addr_len = sizeof(client_addr);

	clients_num = 0;
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
		clients_ready[index] = -1;
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

	printf("Waiting for clients\n");

	while(true){
	
//	sleep(1);
	//clear the socket set
	FD_ZERO(&readfds);
  
	//add master socket to set
	FD_SET(listenfd, &readfds);
	max_fd = listenfd;

	printf("Select... ");	
	for(index = 0; index < MAX_CLIENT_NUM; index ++){
		if(clients_connfd[index] < 0){
			continue;
		}
		printf("%d ",clients_connfd[index]);
		FD_SET(clients_connfd[index], &readfds);
		if(clients_connfd[index] > max_fd){
			max_fd = clients_connfd[index];
		}
	}	
	printf(" .\n");	
	
		res = select(max_fd + 1 , &readfds, NULL , NULL , NULL);
		if(res < 0){
			fprintf(stderr,"Server: Hiba: select %s. \n",strerror(errno));
		}

		// valami történt

		// a listen volt?
		if (FD_ISSET(listenfd, &readfds)){
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

					
					res = add_client_to_dices(dices);
					if(index != res){
						fprintf(stderr,"Server: Hiba: add_client_to_dices res %d  index %d. \n",res, index);
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
				res = send(clients_connfd[index], (void*)&client_ID, sizeof(struct your_ID),0);
				if(res < 0){
					fprintf(stderr,"Server: Hiba: send %d %s. \n",res,strerror(errno));
				}

				break;
			}
		}
		else{
		// a kliens küldött vmit
			//printf("Catch message from clients\n");
			for (index = 0; index < MAX_CLIENT_NUM; index++){
				client = clients_connfd[index];
				if(client < 0){
					continue;
				}
				//printf("Catch message from client %d\n", client);
				// ha nem ez volt akkor megyünk tovább	
				if (FD_ISSET(client, &readfds)){
					printf("Catch message client: %d\n", index);
					res = read(client, recvBuff, sizeof(recvBuff)-1);
					if(res > 0){
						res = process_server_message(state, recvBuff, sizeof(recvBuff),dices);
					}
					else if(res == 0){
						printf("Client disconnected: %d\n", index);
						clients_connfd[index] = -1;
						clients_ready[index] = -1;
						clients_num --;
					}
					else{
						fprintf(stderr,"Server: Hiba: read %d %s. \n",res,strerror(errno));
					}
				}
			}
		}

	}
	

	return 0;
}





