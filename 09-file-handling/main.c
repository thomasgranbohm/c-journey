#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 2048

typedef struct
{
    char *content;
    int size;
} Page;

typedef struct
{
    char *filename;
    long filesize;
    int amount_pages;
    int page_index;
    FILE *file;
    Page *page;
} FileInfo;

long get_file_size(FILE *file)
{
    if (fseek(file, 0, SEEK_END) != 0)
    {
        perror("fseek error");
        exit(EXIT_FAILURE);
    }

    long size = ftell(file);
    if (size == -1)
    {
        perror("ftell error");
        exit(EXIT_FAILURE);
    }

    rewind(file);

    return size;
}

FileInfo init_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("fopen error");
        exit(EXIT_FAILURE);
    }

    long filesize = get_file_size(file);
    int amount_pages = (filesize + PAGE_SIZE - 1) / PAGE_SIZE;

    char *content = malloc(PAGE_SIZE + 1);
    if (content == NULL)
    {
        perror("content malloc error");
        exit(EXIT_FAILURE);
    }

    Page *page = malloc(sizeof(Page));
    if (page == NULL)
    {
        perror("page malloc error");
        exit(EXIT_FAILURE);
    }

    page->content = content;
    page->size = -1;

    FileInfo fileinfo = {filename, filesize, amount_pages, 0, file, page};

    return fileinfo;
}

void load_page(FileInfo *fileinfo, int new_index)
{
    if (new_index == 0 && fileinfo->amount_pages == 0)
        return;

    if (new_index < 0 || new_index >= fileinfo->amount_pages)
    {
        fprintf(stderr, "Page is out of bounds\n");
        exit(EXIT_FAILURE);
    }

    long where_to = new_index * PAGE_SIZE;

    if (where_to >= fileinfo->filesize)
    {
        fprintf(stderr, "Something went wrong trying to change to page %d.\n", new_index);
        exit(EXIT_FAILURE);
    }
    else if (fseek(fileinfo->file, where_to, SEEK_SET) != 0)
    {
        perror("fseek error");
        exit(EXIT_FAILURE);
    }

    char c = 0;
    int i = 0;
    while ((c = fgetc(fileinfo->file)) != EOF && i < PAGE_SIZE)
        fileinfo->page->content[i++] = c;

    fileinfo->page->content[i] = '\0';
    fileinfo->page->size = i < PAGE_SIZE ? i : PAGE_SIZE;
    fileinfo->page_index = new_index;
}

void print_page(FileInfo fileinfo)
{
    int curr = fileinfo.page_index + 1;
    int amount = fileinfo.amount_pages;
    printf("--- Page %d of %d ---\n", curr, amount);
    puts(fileinfo.page->content);
    printf("--- End of page %d ---\n", curr);
}

void get_info(FileInfo *fileinfo)
{
    if (fseek(fileinfo->file, 0, SEEK_SET) == -1)
    {
        fprintf(stderr, "Something went wrong going to the start of the file.\n");
        exit(EXIT_FAILURE);
    }

    int n_chars = 0, n_spaces = 0, n_newlines = 0;
    char c = 0;
    while ((c = getc(fileinfo->file)) && c != EOF)
    {
        n_chars++;
        if (c == ' ')
            n_spaces++;
        if (c == '\n')
            n_newlines++;
    }

    printf("%s has %d characters, %d words, and %d lines.\n", fileinfo->filename, n_chars, n_spaces + 1, n_newlines + 1);

    load_page(fileinfo, fileinfo->page_index);
}

void handle_pages(FileInfo *fileinfo)
{
    char c = 0;
    char buffer[8];
    do
    {
        load_page(fileinfo, fileinfo->page_index);
        print_page(*fileinfo);

        do
        {
            puts("Do you wanna go to the next or the previous page? (n/p/q)");
            fgets(buffer, sizeof(buffer), stdin);
        } while (sscanf(buffer, "%c", &c) != 1);

        switch (c)
        {
        case 'n':
            if (fileinfo->page_index < fileinfo->amount_pages - 1)
                fileinfo->page_index++;

            break;
        case 'p':
            if (fileinfo->page_index > 0)
                fileinfo->page_index--;
            break;
        default:
            break;
        }

    } while (c != 'q');
}

void handle_command(FileInfo *fileinfo)
{
    char buffer[256];
    int i = -1;

    do
    {
        printf("\nHere is a list of available commands:\n\n 0. List information\n 1. Read file\n 2. Quit\n\n");

        do
        {
            printf(" > ");
            fgets(buffer, sizeof(buffer), stdin);
        } while (sscanf(buffer, "%d", &i) != 1 || (i < 0 || i > 2));

        switch (i)
        {
        case 0:
            get_info(fileinfo);
            break;
        case 1:
            handle_pages(fileinfo);
            break;
        }
    } while (i != 2);

    printf("Goodbye!\n");
}

int main(int argc, char *argv[])
{
    char filename[256];

    printf("Welcome to FileReader!\n");

    if (argc > 1)
    {
        strncpy(filename, argv[1], sizeof(filename));
    }
    else
    {
        // I used a seperate buffer before, but now I just use the first one.
        // Idk kinda ugly, less overhead maybe, but probably insecure asf
        printf("Enter filename to load: ");
        do
        {
            fgets(filename, sizeof(filename), stdin);
        } while (sscanf(filename, "%s", filename) != 1);
    }

    FileInfo fileinfo = init_file(filename);

    handle_command(&fileinfo);

    fclose(fileinfo.file);
    free(fileinfo.page->content);
    free(fileinfo.page);

    return 0;
}