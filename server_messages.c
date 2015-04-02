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

#include "server_messages.h"
#include "server_gamelogic.h"
#include "server.h"

int process_server_message(int phase, void* msg, int msglen, int dices[][MAX_DICE_NUM]) {
	int msg_ID ;
	int	client_ID;
	msg_ID = *(int*)msg;
	client_ID = *(int*)(msg + 1);	
	// ------------------------------
	// check if ID matches with phase
	// TODO ez még gány
//	if( ((msg_ID >= 0x10) && (phase != 0)) || 
//			((msg_ID < 0x10 || msg_ID >= 0x30) && (phase != 1)) || 
//			((msg_ID <= 0x30) && (phase != 2))
//		){
//		fprintf(stderr,"Hiba: Server: invalid message Phase %d msgID: %d . \n", phase, msg_ID);
//		return -1;
//	}
	
	printf("Server: Process message: %d\n", msg_ID);
	switch(msg_ID){
		case REG_CLIENT:
			client_reg = *(struct client_reg_msg*)msg;
			printf("Server: Client registered name: %s\n", client_reg.name);
			break;
		default:
			fprintf(stderr,"Hiba: Server: unknown message: %d", msg_ID);
			break;
	}
	
	printf("Server: get message ID %d client %d\n",msg_ID, client_ID);
	return 0;
//	new_dices(dices);
}

// ------------------------
// Accept clients
// ------------------------

//TODO disconnectnél mivan?
void* accept_clients(void* arg){
	int index;
	int found, clients_num;
	int connfd;
	int listenfd;
	struct sockaddr client_addr;
	socklen_t client_addr_len;

	// klienstömb inicializálás
	for(index = 0; index < MAX_CLIENT_NUM; index++)
		clients_connfd[index] = -1;

	listenfd = *(int*)arg;

	printf("Server: Accept thread started \n");
	while(1){

		// TODO blockolora
		//while(connfd < 0){
		printf("Server: Waiting for clients socket %d\n",listenfd);
		if(	(connfd = accept(listenfd, &client_addr, &client_addr_len)) < 0){
			fprintf(stderr,"Server: Hiba: accept %s. \n",strerror(errno));
		} 
		printf("Server: Accept client: %d \n", connfd);
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
				printf("Server: New client added. index: %d fd: %d\n",index, connfd);

			for(index = 0, clients_num = 0; index < MAX_CLIENT_NUM; index++){
				if(clients_connfd[index] > -1){
					clients_num ++;
					break;
				}
			}
				//client_ID = (struct your_ID*)malloc(sizeof(client_ID));
				client_ID.msgID = YOUR_ID;
				client_ID.client_ID = index;
				printf("Server: yourID index: %d\n",index);
				write(clients_connfd[index], (void*)&client_ID, sizeof(struct your_ID));
				//free(client_ID);
				break;
			}
		}
		//if(found){
		//	pthread_cond_signal (&clients_cond);
		//}



		// Az összes kliens csatlakozott
		if(clients_num >= MAX_CLIENT_NUM){
			pthread_mutex_unlock(&clients_mutex);
			break;
		}
		pthread_mutex_unlock(&clients_mutex);
	}
	printf("Server: Accept thread return \n");
}

// ----------------------------
// Receive message from clients
// ----------------------------

void* receive_from_clients(void* arg){
	int n = 0;
	char recvBuff[SOCKET_SIZE];
	int index;
	int clients_num = 0;
	int res;
	int listenfd;

	printf("Server: Receive thread started. Socket: %d\n",listenfd);

	listenfd = *(int*)arg;
	memset(recvBuff, '0',sizeof(recvBuff));
	while(1){
		//pthread_mutex_lock(&client_mutex);
		if((n = read(listenfd, recvBuff, sizeof(recvBuff)-1)) > 0)
		{
			recvBuff[n] = 0;

			// debug
			if(fputs(recvBuff, stdout) == EOF)
			{
					printf("\n Server: Error : Fputs error\n");
			}

			res = process_server_message(state, recvBuff, sizeof(recvBuff),dices);
			continue;
	
		}
		else{
			fprintf(stderr,"Server: Hiba: read %d %s. \n",n,strerror(errno));
			return;
		}
		//pthread_cond_signal (&server_cond);

		
		//pthread_mutex_unlock(&client_mutex);
	}	
	printf("Client: Receive thread return \n");
}

