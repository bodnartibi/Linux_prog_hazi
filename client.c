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
#include "client.h"



/*
void* send_to_server(){
	int index;
	char sendBuff[SOCKET_SIZE];
	int clients_num = 0;

	printf("Client: Send thread started \n");
	while(1){
		pthread_mutex_lock(&server_mutex);
		printf("Client: Send thread wait \n");
		pthread_cond_wait (&server_cond,&server_mutex);
		printf("Client: Send thread wait return \n");


		pthread_mutex_unlock(&server_mutex);
	}	
	printf("Client: Send thread return \n");
}
*/
void* receive_from_server(){
	int n = 0;
	char recvBuff[SOCKET_SIZE];
	int index;
	int clients_num = 0;
	int res;

	printf("Client: Receive thread started \n");
	memset(recvBuff, '0',sizeof(recvBuff));
	while(1){
		//pthread_mutex_lock(&server_mutex);
		if((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
		{
			//recvBuff[n] = 0;

			// debug
			//if(fputs(recvBuff, stdout) == EOF)
			//{
			//		printf("\n Error : Fputs error\n");
			//}
			
			// TODO state-ek
			printf("Client: Get message. size %d \n",n);
			res = process_client_message(0, recvBuff, n);
			continue;
		}
		else{
			fprintf(stderr,"Client: Hiba: read %d %s. \n",n,strerror(errno));
			return;
		}
		//pthread_cond_signal (&server_cond);


		//pthread_mutex_unlock(&server_mutex);
	}	
	printf("Client: Receive thread return \n");
}


int main(int argc, char* argv[]){
	pthread_t receive_thread;
	pthread_t send_thread;

	struct sockaddr_in serv_addr; 

	// check arguments
	if(argc != 3){
		printf("\n Usage: %s <ip of server> <user name (max 15 char)> \n",argv[0]);
			return 1;
	}

	if(strlen(argv[2]) > MAX_NAME_LENGTH){
		printf("\n Usage: %s <ip of server> <user name (max 15 char)> \n",argv[0]);
		return 1;
	}

	strcpy(&my_name[0], (char*)argv[2]);
	printf("Your name: %s \n", my_name);

	pthread_mutex_init(&server_mutex,NULL);
	pthread_cond_init(&server_cond,NULL);

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Error : Could not create socket \n");
		return 1;
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000); 
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0){
		printf("\n inet_pton error occured\n");
		return 1;
	} 

	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
	   printf("\n Error : Connect Failed \n");
	   return 1;
	}

	printf("Client: Connected %d\n", sockfd);

	if(pthread_create(&receive_thread, NULL, receive_from_server, NULL)){
		fprintf(stderr,"Server: Hiba: thread inditas, send_thread %s. \n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	pthread_join(receive_thread, NULL);

	return 0;
}
