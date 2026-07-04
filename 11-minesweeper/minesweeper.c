#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>

#define MIN_BOARD_SIZE 4
#define MAX_BOARD_SIZE 32

#define POINTER_OFFSET(x, y, s) x + (y * s)

enum
{
    UNVISITED,
    VISITED,
    MINE,
    FLAG
};

typedef struct
{
    int size;
    int n_mines;
    char *map;
    char *dist;
} Board;

void print_dist(char *map, int size)
{
    printf("====================\n");
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            printf("%d ", *(map + POINTER_OFFSET(x, y, size)));
        }
        printf("\n");
    }
    printf("====================\n");
}
void print_map(char *map, int size)
{
    printf("====================\n");

    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            char *c;
            switch (*(map + POINTER_OFFSET(x, y, size)))
            {
            case UNVISITED:
                c = "#";
                break;
            case VISITED:
                c = ".";
                break;
            case MINE:
                c = "*";
                break;
            case FLAG:
                c = "!";
                break;
            default:
                c = "A";
                break;
            }

            printf("%c ", *c);
        }
        printf("\n");
    }
    printf("====================\n");
}

void setup_board(Board *bp)
{
    char buffer[64];
    int size;

    do
    {
        printf("Please input board size: ");
        fflush(stdout);
        fgets(buffer, sizeof(buffer), stdin);
        size = atoi(buffer);
    } while (size < MIN_BOARD_SIZE || size > MAX_BOARD_SIZE);

    char *map_pointer = malloc(size * size * sizeof(char));
    char *dist_pointer = malloc(size * size * sizeof(char));

    bp->size = size;
    bp->map = map_pointer;
    bp->dist = dist_pointer;

    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            int addition = POINTER_OFFSET(x, y, size);
            *(map_pointer + addition) = UNVISITED;
            *(dist_pointer + addition) = UNVISITED;
        }
    }
};

void setup_mines(Board *bp)
{
    char buffer[64];
    int n_mines;
    int size = bp->size;

    do
    {
        printf("Please input number of mines: ");
        fflush(stdout);
        fgets(buffer, sizeof(buffer), stdin);
        n_mines = atoi(buffer);
    } while (n_mines < 1 || n_mines > size);

    bp->n_mines = n_mines;

    int *mines = malloc(sizeof(int) * 2 * n_mines);

    for (int i = 0; i < n_mines; i++)
    {
        int x, y;
        char tile;
        do
        {
            x = rand() % size;
            y = rand() % size;
        } while (*(bp->map + POINTER_OFFSET(x, y, size)) == MINE);

        int o = POINTER_OFFSET(x, y, size);

        mines[i * 2] = x;
        mines[i * 2 + 1] = y;

        *(bp->map + o) = MINE;
    }

    for (int i = 0; i < n_mines; i++)
    {

        int x = mines[i * 2];
        int y = mines[i * 2 + 1];

        for (int ny = y - 1; ny <= y + 1; ny++)
        {
            if (ny < 0 || ny > bp->size - 1)
                continue;

            for (int nx = x - 1; nx <= x + 1; nx++)
            {
                if (nx < 0 || nx > bp->size - 1)
                    continue;

                int o = POINTER_OFFSET(nx, ny, bp->size);

                if (*(bp->map + o) == MINE)
                    continue;

                *(bp->dist + o) += 1;
            }
        }
    }

    free(mines);
}

int main(void)
{
    srand(time(NULL));

    Board *board = malloc(sizeof(Board));

    setup_board(board);
    print_map(board->map, board->size);

    setup_mines(board);
    print_map(board->map, board->size);
    print_dist(board->dist, board->size);
}