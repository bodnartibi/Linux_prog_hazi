#ifndef CLIENT_H
#define CLIENT_H


// -------
// Globals
// -------

int sockfd;
char my_name[MAX_NAME_LENGTH];

// server msg
// mutex: server_mutex
// condition: server_con
pthread_mutex_t server_mutex;
pthread_cond_t server_cond;

#endif
