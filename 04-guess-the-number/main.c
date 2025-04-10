#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int MAX = 100;
const int LOW = 1;

int get_rand(int l, int h)
{
    int r = rand();

    return r % ((h - l + 1) + l);
}

int main()
{
    srand(time(NULL));

    int guess;
    int answer = get_rand(LOW, MAX);

    printf("Welcome to guess the number!\nI am thinking of a number between 1 and 100. You will have 8 tries.\nLet's start guessing!\n");

    do
    {
        printf("What is your guess: ");
        scanf("%d", &guess);

        if (guess < answer)
        {
            printf("You need to go higher.\n");
        }
        else if (guess > answer)
        {
            printf("You need to go lower.\n");
        }
    } while (guess != answer);

    printf("Correct, the answer was %d!\n", guess);

    return 0;
}