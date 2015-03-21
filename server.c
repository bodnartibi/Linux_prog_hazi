#include <stdio.h>
#include <errno.h>
#include "messages.h"
#include "server_socket.h"

int main(int argc, char* argv[]){
	int res;
	res = get_message();	
	printf("ez %d\n", res);
	return 0;
}

