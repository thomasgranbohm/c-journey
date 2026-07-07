#include <stdlib.h>
#include <stdio.h>

#include "stack.h"

Stack init_stack()
{
    int max_size = 2;
    int top = -1;
    int *items = malloc(sizeof(int) * max_size);

    Stack stack = {max_size, top, items};

    return stack;
}

void push(Stack *stack, int item)
{
    if (stack->top == stack->max_size - 1)
    {
        stack->items = realloc(stack->items, sizeof(int) * stack->max_size * 2);

        if (stack->items == NULL)
        {
            perror("realloc");
            exit(1);
        }

        stack->max_size *= 2;
    }

    stack->top++;
    stack->items[stack->top] = item;
}

int pop(Stack *stack)
{
    if (stack->top < 0)
        return stack->top;

    int item = stack->items[stack->top];

    stack->top--;

    return item;
}

void free_stack(Stack *stack)
{
    free(stack->items);
}