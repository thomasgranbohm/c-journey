#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

        if (check_state() == WIN)
            draw_win();
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    char buf[4];
    int diff = IMPOSSIBLE;

    if (argc <= 1 || argv[1] == NULL)
    {
        printf("Minesweeper\nPlease select a difficulty!\r\nAvailable difficulties:\r\n1. Easy\r\n2. Medium\r\n3. Hard\r\n\nExample:\r\n./minesweeper 3\n");
        exit(1);
    }
    diff = atoi(argv[1]);

    enable_raw_mode();
    init_terminal();

    setup_game(diff);
    init_game();

    game_loop();
}