#ifndef GAME_H
#define GAME_H

enum
{
    UNVISITED,
    VISITED,
    PLAIN,
    MINE,
    FLAG
};

typedef struct
{
    int size;
    int n_mines;
    char *map;
    char *visited;
    char *dist;
} Board;

void reveal_tile(int x, int y);
void setup_board();
void setup_mines();

#endif