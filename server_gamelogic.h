// TODO hack: tudni kellene mekkora a legnagyobb üzenet
#define READ_SIZE 1024

#include "messages.h"

void init_dices(int dices_array[][MAX_DICE_NUM]);
void new_dices(int dices_array[][MAX_DICE_NUM]);
int add_client_to_dices(int dices_array[][MAX_DICE_NUM]);
int check_challenge(int face, int quantity,int dices_array[][MAX_DICE_NUM]);
