#include "messages.h"
#include <stdio.h>

void game_logic_test() {
	int res, client, dice, face, quan;
	int fd;
	char * msg;
	int ID;
	int dices[MAX_CLIENT_NUM][MAX_DICE_NUM];

	for(client = 0; client < MAX_CLIENT_NUM; client ++)
	for(dice = 0; dice < MAX_DICE_NUM; dice ++)
	dices[client][dice] = -2;

	printf("Add clients \n");

	res = add_client_to_dices(dices);
	printf("res %d\n", res);

	res = add_client_to_dices(dices);
	printf("res %d\n", res);

	printf("Feltöltött kockák\n");
	new_dices(dices);
	for(client = 0; client < MAX_CLIENT_NUM; client ++){
		printf("\nKliens %d: ",client);
		for(dice = 0; dice < MAX_DICE_NUM; dice ++){
			printf(" %d", dices[client][dice]);
		}
	}

	printf("\nKliens kockáinak visszavonása\n");
	res = remove_client_dices(1, 2, dices);
	printf("res %d\n", res);
	for(client = 0; client < MAX_CLIENT_NUM; client ++){
		printf("\nKliens %d: ",client);
		for(dice = 0; dice < MAX_DICE_NUM; dice ++){
			printf(" %d", dices[client][dice]);
		}
	}

	printf("\nChallenge: \n");
	scanf("%d",&face);
	scanf("%d",&quan);
	res = check_challenge(face, quan, dices);
	printf("res %d\n",res);

}
