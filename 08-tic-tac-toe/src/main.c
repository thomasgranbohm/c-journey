#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "game.h"
#include "types.h"
#include "ui.h"

int start_game()
{
    Occupation *board = malloc(sizeof(Occupation) * MAX_CELLS);

    if (board == NULL)
    {
        puts("Something went wrong when allocating memory.");
        return 1;
    }

    welcome();

    char choice = 'y';
    while (choice == 'y' || choice == 'Y')
    {
        init_board(board);
        int turn = 0;

        print_board(board);

        while (turn < MAX_TURNS)
        {
            Occupation player = 1 + (turn % 2);
            Position pos = take_input(player);

            if (!can_place(board, pos))
                continue;

            place_player(board, player, pos);
            print_board(board);

            Occupation winner = check_win(board);

            if (winner != EMPTY)
            {
                win(winner);
                break;
            }
            turn++;

            if (turn == MAX_TURNS)
                draw();
        }

        restart();

        get_choice(&choice);
    }

    free(board);

    return 0;
}

int main()
{
    return start_game();
}