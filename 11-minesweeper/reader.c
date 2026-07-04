#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "game.h"

extern Board *board;

#define CTRL_KEY(k) (k & 0x1f)

struct wbuf
{
    char *b;
    int len;
};

#define WBUF_INIT {NULL, 0}

void wAppend(struct wbuf *wb, const char *s, int len)
{
    char *new = realloc(wb->b, wb->len + len);

    if (new == NULL)
        return;

    memcpy(&new[wb->len], s, len);
    wb->b = new;
    wb->len += len;
}

void wFree(struct wbuf *wb)
{
    free(wb->b);
}

struct config
{
    int cx, cy;
    struct termios orig;
};

struct config E;

void die(const char *s)
{
    perror(s);
    exit(1);
}

void disable_raw_mode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig) == -1)
        die("tcsetattr");
}

void enable_raw_mode()
{
    if (tcgetattr(STDIN_FILENO, &E.orig) == -1)
        die("tcgetattr");

    atexit(disable_raw_mode);

    struct termios raw = E.orig;

    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

char read_key()
{
    int nread;
    char c;

    while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if (nread == -1 && errno != EAGAIN)
            die("read");
    }

    return c;
}

void draw_map(struct wbuf *wb)
{
    for (int y = 0; y < board->size; y++)
    {
        for (int x = 0; x < board->size; x++)
        {
            char c;
            int offset = x + y * board->size;
            switch (board->visited[offset])
            {
            case UNVISITED:
                c = '#';
                break;
            case VISITED:
                switch (board->map[offset])
                {
                case PLAIN:
                    int dist = board->dist[offset];
                    if (dist != 0)
                        c = dist + 48;
                    else
                        c = '.';
                    break;
                case FLAG:
                    c = '!';
                    break;
                case MINE:
                    c = 'O';
                    break;
                }
                break;
            }
            wAppend(wb, &c, 1);
        }
        wAppend(wb, "\r\n", 2);
    }
}

void refresh_screen()
{
    struct wbuf wb = WBUF_INIT;
    wAppend(&wb, "\x1b[2J", 4);
    wAppend(&wb, "\x1b[H", 3);

    draw_map(&wb);

    char buf[32];
    snprintf(buf, sizeof(buf), "%d:%d", E.cx + 1, E.cy + 1);
    wAppend(&wb, buf, strlen(buf));

    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
    wAppend(&wb, buf, strlen(buf));

    write(STDOUT_FILENO, wb.b, wb.len);
    wFree(&wb);
}

void move_cursor(char key)
{
    switch (key)
    {
    case 'w':
        if (E.cy != 0)
            E.cy--;
        break;
    case 'a':
        if (E.cx != 0)
            E.cx--;
        break;
    case 's':
        if (E.cy != 64)
            E.cy++;
        break;
    case 'd':
        if (E.cx != 64)
            E.cx++;
        break;
    }
}

void process_key()
{
    char c = read_key();

    switch (c)
    {
    case 'w':
    case 'a':
    case 's':
    case 'd':
        move_cursor(c);
        break;
    case ' ':
        reveal_tile(E.cy, E.cx);
        break;
    case 'q':
        exit(0);
        break;
    }
}

void init_terminal()
{
    E.cx = 0;
    E.cy = 0;
}