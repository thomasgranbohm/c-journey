#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>

#include "game.h"
#include "reader.h"
#include "stack.h"

Board *board;

int main(void)
{
    srand(time(NULL));

    board = malloc(sizeof(Board));

    setup_board();
    setup_mines();

    enable_raw_mode();
    init_terminal();

    // Game loop kinda
    while (1)
    {
        refresh_screen();
        process_key();
        check_win();
    }
}