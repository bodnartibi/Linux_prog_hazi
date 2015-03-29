#ifndef SERVER_H
#define SERVER_H

// -------
// Globals
// -------

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
pthread_mutex_t clients_mutex;
pthread_cond_t clients_cond;


// state-machine
int state;
pthread_mutex_t state_mutex;
pthread_cond_t state_cond;


#endif
