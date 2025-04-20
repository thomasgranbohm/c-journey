#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ui.h"

char get_player(Occupation player)
{
    switch (player)
    {
    case NOUGHT:
        return 'o';
    case CROSS:
        return 'x';
    default:
        return ' ';
    }
}

void print_board(Occupation *board)
{
    // Huuuwee ugly
    printf("\n    0   1   2\n");
    printf("  +---+---+---+\n");
    printf("0 | %c | %c | %c |\n", get_player(board[0]), get_player(board[1]), get_player(board[2]));
    printf("  +---+---+---+\n");
    printf("1 | %c | %c | %c |\n", get_player(board[3]), get_player(board[4]), get_player(board[5]));
    printf("  +---+---+---+\n");
    printf("2 | %c | %c | %c |\n", get_player(board[6]), get_player(board[7]), get_player(board[8]));
    printf("  +---+---+---+\n\n");
}

void get_choice(char *c)
{
    while (1)
    {
        char buffer[128];
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%c", c) == 1)
            break;
    }
}

Position take_input(Occupation player)
{
    printf("%c's turn (x y) > ", player == NOUGHT ? 'o' : 'x');
    do
    {
        char buffer[128];
        int x, y;
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d %d", &x, &y) == 2)
        {
            return (Position){x, y};
        }
    } while (1);
}

void welcome()
{
    puts("Welcome to Tic-Tac-Toe!");
    puts("Please input your placements as \"x y\", for example \"2 1\".");
    puts("Good luck!");
}

void win(Occupation winner)
{
    printf("%s wins!\n", winner == NOUGHT ? "Naught" : "Cross");
}

void draw()
{
    printf("We have a draw!\n");
}

void restart()
{
    printf("Restart? (y/n) ");
}
