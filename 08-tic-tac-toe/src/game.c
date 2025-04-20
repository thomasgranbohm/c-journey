#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "game.h"

void init_board(Occupation *board)
{
    for (int i = 0; i < MAX_CELLS; i++)
    {
        board[i] = EMPTY;
    }
}

int pos_to_index(Position pos)
{
    return pos.x + (pos.y * BOARD_SIZE);
}

void place_player(Occupation *board, Occupation player, Position pos)
{
    board[pos_to_index(pos)] = player;
}

int can_place(Occupation *board, Position pos)
{
    int index = pos_to_index(pos);

    if (index < 0 || index >= MAX_CELLS)
    {
        printf("Invalid position.\n");
        return 0;
    }

    if (board[index] != EMPTY)
    {
        printf("Position is already occupied.\n");
        return 0;
    }

    return 1;
}

// Very ugly
Occupation check_win(Occupation *board)
{
    // Checking columns
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (board[i] != EMPTY && board[i] == board[i + BOARD_SIZE] && board[i] == board[i + 2 * BOARD_SIZE])
        {
            return board[i];
        }
    }

    // Checking rows wins
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int l = i * BOARD_SIZE;

        if (board[l] != EMPTY && board[l] == board[l + 1] && board[l] == board[l + 2])
        {
            return board[l];
        }
    }

    // Checking diagonals
    if (board[0] != EMPTY && board[0] == board[4] && board[0] == board[8])
    {
        return board[0];
    }
    else if (board[2] != EMPTY && board[2] == board[4] && board[2] == board[6])
    {
        return board[2];
    }

    return 0;
}
