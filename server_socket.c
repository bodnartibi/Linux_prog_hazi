int init_communication(){
	int rcv_fd;
  rcv_fd = open("to_server", O_RDONLY | O_NONBLOCK);
  if(rcv_fd < 0)
  {
    perror("open");
    return EXIT_FAILURE;
  }

	return rcv_fd;
}

int get_message() {

 
 return 2;
}
