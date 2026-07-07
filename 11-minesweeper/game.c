#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "game.h"
#include "stack.h"

#define POINTER_OFFSET(x, y, s) (x + (y * s))

extern Board *board;

void free_board()
{
    free(board->map);
    free(board->visited);
    free(board->dist);
    free(board);
}

void setup_board(int size)
{
    board = malloc(sizeof(Board));
    if (!board)
    {
        perror("malloc board failed");
        exit(1);
    }

    char *map_pointer = malloc(size * size * sizeof(char));
    char *visited_pointer = malloc(size * size * sizeof(char));
    char *dist_pointer = malloc(size * size * sizeof(char));

    if (!map_pointer)
    {
        perror("malloc map_pointer failed");
        exit(1);
    }
    if (!visited_pointer)
    {
        perror("malloc visited_pointer failed");
        exit(1);
    }
    if (!dist_pointer)
    {
        perror("malloc dist_pointer failed");
        exit(1);
    }

    board->size = size;
    board->map = map_pointer;
    board->visited = visited_pointer;
    board->dist = dist_pointer;
    board->n_visited = 0;

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

void setup_mines(int n_mines)
{
    int size = board->size;
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

void setup_game(enum Difficulty diff)
{
    int size, n_mines;
    switch (diff)
    {
    case EASY:
        size = 9;
        n_mines = 10;
        break;
    case MEDIUM:
        size = 16;
        n_mines = 40;
        break;
    case HARD:
        size = 36;
        n_mines = 99;
        break;

    default:
        write(STDERR_FILENO, "Unknown difficulty\r\n", 20);
        exit(1);
        break;
    }

    setup_board(size);
    setup_mines(n_mines);

    atexit(free_board);
}

enum GameState reveal_tile(int x, int y)
{
    int curr = POINTER_OFFSET(x, y, board->size);

    if (x < 0 || x >= board->size || y < 0 || y >= board->size)
        return PENDING;

    if (board->visited[curr] == VISITED)
        return PENDING;

    board->visited[curr] = VISITED;

    if (board->map[curr] == MINE)
        return LOSS;

    if (board->map[curr] != PLAIN)
        return PENDING;

    else if (board->dist[curr] != 0)
    {
        board->n_visited++;
        return PENDING;
    }

    Stack s = init_stack();
    push(&s, curr);

    while (s.top != -1)
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