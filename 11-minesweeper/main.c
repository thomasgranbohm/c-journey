#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>

#include "game.h"
#include "reader.h"
#include "stack.h"

Board *board;

void game_loop()
{
    while (1)
    {
        refresh_screen();
        process_key();

        if (check_win() == WIN)
            draw_win();
    }
}

int main(void)
{
    srand(time(NULL));

    board = malloc(sizeof(Board));

    setup_board();
    setup_mines();

    enable_raw_mode();
    init_terminal();

    game_loop();
}