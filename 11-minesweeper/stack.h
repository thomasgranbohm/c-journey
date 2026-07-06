#ifndef STACK_H
#define STACK_H
typedef struct
{
    int max_size;
    int top;
    int *items;
} Stack;

Stack init_stack();
void push(Stack *stack, int item);
int pop(Stack *stack);
void free_stack(Stack *stack);
#endif