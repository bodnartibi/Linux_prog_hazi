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
int clients_connfd[MAX_CLIENT_NUM];
int clients_ready[MAX_CLIENT_NUM];
char client_names[MAX_CLIENT_NUM][MAX_NAME_LENGTH];

#endif
