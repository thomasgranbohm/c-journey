#ifndef GAME_H
#define GAME_H

enum Difficulty
{
    EASY = 1,
    MEDIUM = 2,
    HARD = 3
};

#define DIFF_EASY {9, 10}
#define DIFF_MEDIUM {16, 40}
#define DIFF_HARD {36, 99}

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
enum GameState check_win();

void free_board();

#endif