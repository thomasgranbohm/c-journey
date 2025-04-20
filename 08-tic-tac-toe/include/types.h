#ifndef TICTACTOE_TYPES_H
#define TICTACTOE_TYPES_H

#define BOARD_SIZE 3
#define MAX_CELLS 9
#define MAX_TURNS 9

typedef struct
{
    int x;
    int y;
} Position;

typedef enum
{
    EMPTY = 0,
    NOUGHT = 1,
    CROSS = 2,
} Occupation;

#endif