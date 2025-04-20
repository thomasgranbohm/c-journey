#include "types.h"

char get_player(Occupation player);
void print_board(Occupation *board);
Position take_input(Occupation player);
void get_choice(char *c);

void welcome();
void win(Occupation player);
void draw();
void restart();