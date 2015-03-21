#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include "messages.h"
#include "server_socket.h"

int main(int argc, char* argv[]){
	int res;
	int fd;
	fd = init_communication();
	res = get_message(fd);	
	printf("ez %d\n", res);
	return 0;
}

