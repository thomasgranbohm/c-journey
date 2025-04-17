#include <stdio.h>

void win()
{
    printf("Wow you won");
}

void vuln()
{
    char input[64];
    printf("Please input a string (64 chars max): ");
    scanf("%s", input);
    printf("You entered %s\n", input);
}

int main()
{
    vuln();
    return 0;
}