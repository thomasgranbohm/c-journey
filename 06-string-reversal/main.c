#include <stdio.h>

const int AMOUNT = 64;

int custom_strlen(char string[AMOUNT])
{
    for (int i = 0; i < AMOUNT; i++)
    {
        char c = string[i];
        if (c == '\n' || c == '\0')
        {
            return i;
        }
    }

    return AMOUNT;
}

void reverse_string_using_indexes(char string[AMOUNT])
{
    int EOS = custom_strlen(string);
    string[EOS] = '\0';

    for (int i = 0, e = EOS - 1; i < e; i++, e--)
    {
        char a = string[i];
        string[i] = string[e];
        string[e] = a;
    }

    printf("Reversed string using indexes: %s\n", string);
}

void reverse_string_using_pointers(char string[AMOUNT])
{
    int EOS = custom_strlen(string);
    string[EOS] = '\0';

    char *i = string;
    char *e = i + EOS - 1;

    while (i < e)
    {
        char temp = *i; // Temp is an address, the same address ad i
        *i = *e;        // Change the value stored at i to the value stored at e
        *e = temp;      // Change the value stored at e to the value stored at e

        i++;
        e--;
    }

    printf("Reversed string using pointers: %s\n", string);
}

int main()
{
    char string[AMOUNT];

    printf("Please string a string (max %d chars): ", AMOUNT);
    fgets(string, sizeof(string), stdin);

    reverse_string_using_pointers(string);
    reverse_string_using_pointers(string);

    return 0;
}