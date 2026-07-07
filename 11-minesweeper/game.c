#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "game.h"
#include "stack.h"

#define MIN_BOARD_SIZE 4
#define MAX_BOARD_SIZE 32

#define POINTER_OFFSET(x, y, s) x + (y * s)

extern Board *board;

void setup_board()
{
    char buffer[64];
    int size = 32;

    do
    {
        printf("Please input board size: ");
        fflush(stdout);
        fgets(buffer, sizeof(buffer), stdin);
        size = atoi(buffer);
    } while (size < MIN_BOARD_SIZE || size > MAX_BOARD_SIZE);

    char *map_pointer = malloc(size * size * sizeof(char));
    char *visited_pointer = malloc(size * size * sizeof(char));
    char *dist_pointer = malloc(size * size * sizeof(char));

    board->size = size;
    board->map = map_pointer;
    board->visited = visited_pointer;
    board->dist = dist_pointer;

    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            int addition = POINTER_OFFSET(x, y, size);
            *(map_pointer + addition) = PLAIN;
            *(visited_pointer + addition) = UNVISITED;
            *(dist_pointer + addition) = 0;
        }
    }
};

void setup_mines()
{
    char buffer[64];
    int n_mines = 99;
    int size = board->size;

    do
    {
        printf("Please input number of mines: ");
        fflush(stdout);
        fgets(buffer, sizeof(buffer), stdin);
        n_mines = atoi(buffer);
    } while (n_mines < 1 || n_mines > size);

    board->n_mines = n_mines;

    int *mines = malloc(sizeof(int) * 2 * n_mines);

    for (int i = 0; i < n_mines; i++)
    {
        int x, y;
        char tile;
        do
        {
            x = rand() % size;
            y = rand() % size;
        } while (*(board->map + POINTER_OFFSET(x, y, size)) == MINE);

        int o = POINTER_OFFSET(x, y, size);

        mines[i * 2] = x;
        mines[i * 2 + 1] = y;

        *(board->map + o) = MINE;
    }

    for (int i = 0; i < n_mines; i++)
    {

        int x = mines[i * 2];
        int y = mines[i * 2 + 1];

        for (int ny = y - 1; ny <= y + 1; ny++)
        {
            if (ny < 0 || ny > board->size - 1)
                continue;

            for (int nx = x - 1; nx <= x + 1; nx++)
            {
                if (nx < 0 || nx > board->size - 1)
                    continue;

                int o = POINTER_OFFSET(nx, ny, board->size);

                if (*(board->map + o) == MINE)
                    continue;

                *(board->dist + o) += 1;
            }
        }
    }

    free(mines);
}

enum GameState reveal_tile(int x, int y)
{
    int curr = POINTER_OFFSET(x, y, board->size);

    board->visited[curr] = VISITED;

    if (board->map[curr] == MINE)
    {
        return LOSS;
    }
    if (board->map[curr] != PLAIN)
    {
        return PENDING;
    }
    else if (board->dist[curr] != 0)
    {
        board->n_visited++;
        return PENDING;
    }

    Stack s = init_stack();
    push(&s, curr);

    while (s.top != 0)
    {
        curr = pop(&s);
        x = curr % board->size;
        y = curr / board->size;

        board->n_visited++;

        int pos;
        for (int cy = y - 1; cy <= y + 1; cy++)
        {
            if (cy < 0 || cy >= board->size)
                continue;
            for (int cx = x - 1; cx <= x + 1; cx++)
            {
                if (cx < 0 || cx >= board->size)
                    continue;

                pos = POINTER_OFFSET(cx, cy, board->size);

                if (pos == curr)
                    continue;
                if (board->visited[pos] == VISITED)
                    continue;

                board->visited[pos] = VISITED;

                if (board->dist[pos] == 0)
                    push(&s, pos);
                else
                    board->n_visited++;
            }
        }
    }

    free_stack(&s);

    return PENDING;
}

void place_flag(int x, int y)
{
    int offset = POINTER_OFFSET(x, y, board->size);
    int output = FLAG;

    if (board->visited[offset] == VISITED)
        return;

    if (board->visited[offset] == FLAG)
        output = UNVISITED;

    board->visited[offset] = output;
}

enum GameState check_win()
{
    if (board->n_visited == board->size * board->size - board->n_mines)
    {
        return WIN;
    }
    return PENDING;
}
