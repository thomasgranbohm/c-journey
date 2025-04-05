#include <stdio.h>

int main()
{
    char name[20];

    printf("Please input your name (max 20 characters): ");
    scanf("%s", name);
    printf("Hello from scanf, %s!\n", name);

    /*
    scanf does not read the newline from the input buffer,
    so its left there. Thats why we need to clean it up
    before we run fgets.
    */

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // Flush input buffer

    printf("Please input your name (max 20 characters): ");
    fgets(name, sizeof(name), stdin);
    printf("Hello from fgets, %s!\n", name);

    return 0;
}