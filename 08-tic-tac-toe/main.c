#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BOARD_SIZE 3
#define MAX_CELLS 9
#define MAX_TURNS 9

typedef struct
{
    uint8_t x;
    uint8_t y;
} Position;

typedef enum
{
    EMPTY = 0,
    NOUGHT = 1,
    CROSS = 2,
} Occupation;

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

char get_player(Occupation player)
{
    char c;
    switch (player)
    {
    case NOUGHT:
        c = 'o';
        break;
    case CROSS:
        c = 'x';
        break;
    default:
        c = '-';
    }

    return c;
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