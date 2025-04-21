#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 512

typedef struct
{
    char *content;
    int size;
    int index;
} Page;

typedef struct
{
    char *filename;
    long filesize;
    FILE *file;
    Page *page;
} FileInfo;

long get_file_size(FILE *file)
{
    if (fseek(file, 0, SEEK_END) != 0)
    {
        fprintf(stderr, "Something went wrong going to the end of the file.\n");
        exit(EXIT_FAILURE);
    }

    long size = ftell(file);
    if (size == -1)
    {
        fprintf(stderr, "ftell failed\n");
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
        fprintf(stderr, "Something went wrong opening the file.\n");
        exit(EXIT_FAILURE);
    }

    long filesize = get_file_size(file);
    char *content = malloc(sizeof(char) * PAGE_SIZE);

    Page page = {content, -1, 0};
    FileInfo fileinfo = {filename, filesize, file, &page};

    return fileinfo;
}

void load_page(FileInfo *fileinfo, int index)
{
    long where_to = index * PAGE_SIZE;

    if (where_to >= fileinfo->filesize || fseek(fileinfo->file, 0, where_to) != 0)
    {
        fprintf(stderr, "Something went wrong trying to change to page %ld.\n", where_to);
        exit(EXIT_FAILURE);
    }

    char c = 0;
    int i = 0;
    while ((c = fgetc(fileinfo->file)) && c != EOF && i < PAGE_SIZE)
    {
        fileinfo->page->content[i++] = c;
        // i++;
    }

    (*(*fileinfo).page).size = i < PAGE_SIZE ? i : PAGE_SIZE;
}

void print_page(FileInfo fileinfo)
{
    printf("--- Start of %d of %s ---\n", fileinfo.page->size, fileinfo.filename);
    printf("%d\n", fileinfo.page->size);
    for (int i = 0; i < fileinfo.page->size; i++)
    {
        printf("\n%d %d\n", i, fileinfo.page->size);
        putc(*(fileinfo.page->content + i), stdout);
    }

    printf("\n--- End of page %d of %s ---\n", fileinfo.page->index, fileinfo.filename);
}

int main(int argc, char *argv[])
{
    char filename[256];
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

    load_page(&fileinfo, 0);

    print_page(fileinfo);

    return 0;
}