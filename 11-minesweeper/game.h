#ifndef GAME_H
#define GAME_H

enum Difficulty
{
    EASY = 1,
    MEDIUM = 2,
    HARD = 3,
    IMPOSSIBLE = 4,
};

enum Tiles
{
    UNVISITED,
    VISITED,
    PLAIN,
    MINE,
    FLAG
};

enum GameState
{
    PENDING,
    WIN,
    LOSS,
};

typedef struct
{
    int size;
    int n_mines;
    int n_visited;
    char *map;
    char *visited;
    char *dist;
} Board;

enum GameState reveal_tile(int x, int y);
void place_flag(int x, int y);
void setup_board(int size);
void setup_mines(int n_mines);
void setup_game(enum Difficulty);
enum GameState check_state();

void free_board();

#endif