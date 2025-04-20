#include "types.h"

void init_board(Occupation *board);
void place_player(Occupation *board, Occupation player, Position pos);
int can_place(Occupation *board, Position pos);
Occupation check_win(Occupation *board);
int pos_to_index(Position pos);