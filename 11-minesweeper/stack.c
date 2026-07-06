#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    int max_size;
    int top;
    int *items;
} Stack;

Stack init_stack()
{
    int max_size = 2;
    int top = 0;
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
    if (stack->top == 0)
        return -1;

    int item = stack->items[stack->top];

    stack->top--;

    return item;
}

void free_stack(Stack *stack)
{
    free(stack->items);
}