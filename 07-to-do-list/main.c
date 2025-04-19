#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char description[128];
    int done;
} Task;

typedef enum
{
    ADD = 1,
    REMOVE = 2,
    LIST = 3,
    TOGGLE = 4,
    EXIT = 5
} Command;

void add_note(Task **tasks, int *amount, int *capacity)
{
    char description[128];

    printf("Enter task description: ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = '\0';

    if (*amount == *capacity)
    {
        ++(*capacity);
        (*tasks) = realloc((*tasks), sizeof(Task) * *capacity);
    }

    strncpy((*tasks)[*amount].description, description, sizeof((*tasks)[*amount].description));
    (*amount)++;
}

void list_tasks(Task *tasks, int amount)
{
    if (amount == 0)
    {
        printf("You have no tasks in your list.\n");
        return;
    }

    puts("Your tasks:");
    for (int i = 0; i < amount; i++)
    {
        printf("[%d] [%c] %s\n", i + 1, tasks[i].done > 0 ? 'x' : ' ', tasks[i].description);
    }
}

void toggle_status(Task **tasks, int amount)
{
    if (amount == 0)
    {
        printf("You have no tasks in your list.\n");
        return;
    }

    list_tasks(*tasks, amount);

    printf("Enter task index to mark as done: ");

    char buffer[128];
    int i;
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%d", &i) != 1)
    {
        printf("Invalid input. Please input a number!\n");
        return;
    }

    i--;

    if (i < 0 || i >= amount)
    {
        printf("That task does not exist.\n");
        return;
    }

    int is_done = (*tasks)[i].done;

    (*tasks)[i].done = is_done > 0 ? 0 : 1;
}

void remove_task(Task **tasks, int *amount)
{
    if (*amount == 0)
    {
        printf("You have no tasks in your list.\n");
        return;
    }

    list_tasks(*tasks, *amount);

    printf("Enter task index to remove: ");

    char buffer[128];
    int i;
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%d", &i) != 1)
    {
        printf("Invalid input. Please input a number!\n");
        return;
    }

    i--;

    if (i < 0 || i >= *amount)
    {
        printf("That task does not exist.\n");
        return;
    }

    for (; i < *amount - 1; i++)
    {
        strncpy((*tasks)[i].description, (*tasks)[i + 1].description, sizeof((*tasks)[i].description));
        (*tasks)[i].done = (*tasks)[i + 1].done;
    }

    (*amount)--;
}

int main()
{
    Task *tasks = NULL;
    int amount = 0;
    int capacity = 0;

    int command = 0;

    puts("Welcome to the Dynamic To-do list!");
    puts("");
    puts("1. Add task");
    puts("2. Remove task");
    puts("3. List tasks");
    puts("4. Mark task as done");
    puts("5. Exit");

    while (command != 5)
    {
        printf("\nWhat do you want to do?\n> ");

        char buffer[128];
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &command) != 1)
        {
            printf("Invalid command.\n");
            continue;
        }

        switch (command)
        {
        case ADD:
            // Send the pointers of tasks, amount, and capacity to the function
            add_note(&tasks, &amount, &capacity);
            break;
        case REMOVE:
            remove_task(&tasks, &amount);
            break;
        case LIST:
            list_tasks(tasks, amount);
            break;
        case TOGGLE:
            toggle_status(&tasks, amount);
            break;
        case EXIT:
            break;
        default:
            printf("Invalid command.\n");
            break;
        }
    }

    puts("Goodbye!");
    free(tasks);

    return 0;
}