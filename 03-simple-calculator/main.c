#include <stdio.h>

int main()
{
    char operator;
    int i_a, i_b, o;

    printf("Please input query:\n");
    scanf("%d %c %d", &i_a, &operator, & i_b);

    switch (operator)
    {
    case '+':
        o = i_a + i_b;
        break;
    case '-':
        o = i_a - i_b;
        break;
    case '*':
        o = i_a * i_b;
        break;
    case '/':
        o = i_a / i_b;
        break;
    case '%':
        o = i_a % i_b;
        break;
    default:
        fprintf(stderr, "%s", "Operator not implemented.\n");
        return 1;
    }

    printf("Output: %d\n", o);

    return 0;
}