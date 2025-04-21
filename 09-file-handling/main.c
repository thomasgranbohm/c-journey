#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 2048
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

    Page *page = malloc(sizeof(Page));
    page->content = content;
    page->index = 0;
    page->size = -1;

    FileInfo fileinfo = {filename, filesize, file, page};

    return fileinfo;
}

void load_page(FileInfo *fileinfo, int index)
{
    long where_to = index * PAGE_SIZE;

    if (where_to >= fileinfo->filesize || fseek(fileinfo->file, where_to, SEEK_SET) != 0)
    {
        fprintf(stderr, "Something went wrong trying to change to page %d.\n", index);
        exit(EXIT_FAILURE);
    }

    char c = 0;
    int i = 0;
    while ((c = fgetc(fileinfo->file)) && c != EOF && i < PAGE_SIZE)
    {
        fileinfo->page->content[i++] = c;
    }

    fileinfo->page->size = i < PAGE_SIZE ? i : PAGE_SIZE;
    fileinfo->page->index = index;
}

void print_page(FileInfo fileinfo)
{
    printf("--- Start of page %d of %s ---\n", fileinfo.page->index, fileinfo.filename);
    for (int i = 0; i < fileinfo.page->size; i++)
    {
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

    fclose(fileinfo.file);
    free(fileinfo.page->content);
    free(fileinfo.page);

    return 0;
}