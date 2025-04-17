#include <stdint.h>
#include <stdio.h>

const uint32_t AMOUNT_OF_NUMBERS = 10;

uint32_t sum(uint32_t numbers[AMOUNT_OF_NUMBERS])
{
    uint32_t s = 0;

    for (uint32_t i = 0; i < AMOUNT_OF_NUMBERS; i++)
    {
        s += numbers[i];
    }

    return s;
}

// Doesn't really make sense to have this since
// the numbers isn't ordered.
float mean(uint32_t numbers[AMOUNT_OF_NUMBERS])
{
    float d = AMOUNT_OF_NUMBERS / 2;
    uint32_t f = AMOUNT_OF_NUMBERS % 2;

    if (f == 1)
    {
        return (float)numbers[(uint32_t)d];
    }
    else
    {
        uint32_t l = numbers[(uint32_t)(d - 0.5)];
        uint32_t h = numbers[(uint32_t)(d + 0.5)];

        return (float)(h + l) / 2;
    }
}

float average(uint32_t numbers[AMOUNT_OF_NUMBERS])
{
    uint32_t s = sum(numbers);

    return ((float)s / AMOUNT_OF_NUMBERS);
}

uint32_t min(uint32_t numbers[AMOUNT_OF_NUMBERS])
{
    uint32_t M = UINT32_MAX;

    for (uint32_t i = 0; i < AMOUNT_OF_NUMBERS; i++)
    {
        if (numbers[i] < M)
        {
            M = numbers[i];
        }
    }

    return M;
}
uint32_t max(uint32_t numbers[AMOUNT_OF_NUMBERS])
{
    uint32_t M = 0;

    for (uint32_t i = 0; i < AMOUNT_OF_NUMBERS; i++)
    {
        if (numbers[i] > M)
        {
            M = numbers[i];
        }
    }

    return M;
}

int main()
{
    uint32_t numbers[AMOUNT_OF_NUMBERS];
    printf("Please input %d numbers:\n", AMOUNT_OF_NUMBERS);

    for (uint32_t i = 0; i < AMOUNT_OF_NUMBERS; i++)
    {
        scanf("%d", &numbers[i]);
    }

    printf("Sum of array: %d\n", sum(numbers));
    printf("Mean of array: %.2f\n", mean(numbers));
    printf("Average of array: %.2f\n", average(numbers));
    printf("Min of array: %d\n", min(numbers));
    printf("Max of array: %d\n", max(numbers));

    return 0;
}