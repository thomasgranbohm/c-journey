#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
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
    int rows, cols;
    int cx, cy;
    int ax, ay;

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
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.rows + 1, 0);
    write(STDOUT_FILENO, buf, strlen(buf));

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

int get_window_size(int *rows, int *cols)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    {
        return -1;
    }
    else
    {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

void init_terminal()
{
    if (get_window_size(&E.rows, &E.cols) == -1)
        die("get_window_size");

    E.ay = (E.rows - board->size) / 2;
    E.ax = (E.cols - board->size) / 2;

    E.cx = 0;
    E.cy = 0;
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
    char buf[32];
    char c;
    int dist;
    for (int y = 0; y < board->size; y++)
    {
        snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.ay + y, E.ax);
        wAppend(wb, buf, strlen(buf));
        for (int x = 0; x < board->size; x++)
        {
            int offset = x + y * board->size;
            switch (board->visited[offset])
            {
            case FLAG:
                c = 'F';
                break;
            case UNVISITED:
                c = '#';
                break;
            case VISITED:
                switch (board->map[offset])
                {
                case PLAIN:
                    dist = board->dist[offset];
                    if (dist != 0)
                    {
                        snprintf(buf, sizeof(buf), "\x1b[1;34;%dm", 30 + dist);
                        wAppend(wb, buf, strlen(buf));

                        c = dist + 48;
                    }
                    else
                        c = '.';
                    break;
                case MINE:
                    c = 'O';
                    break;
                }
                break;
            }
            wAppend(wb, &c, 1);

            if (dist != 0)
            {
                strcpy(buf, "\x1b[0;34;39m");
                wAppend(wb, buf, strlen(buf));
            }
        }
    }
}

void draw_centered_string(struct wbuf *wb, char *s, int y)
{
    int x = (E.cols - strlen(s)) / 2;
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y, x);
    wAppend(wb, buf, strlen(buf));
    wAppend(wb, s, strlen(s));
}

void draw_information(struct wbuf *wb)
{
    draw_centered_string(wb, "Minesweeper", E.ay - 2);

    draw_centered_string(wb, "(q) Quit    (w/a/s/d) Movement    (f) Toggle flag    (space) Reveal tile", E.ay + board->size + 2);
}

void draw_endgame(enum GameState status)
{
    struct wbuf wb = WBUF_INIT;

    int y = E.ay + board->size + 2;
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y, 1);
    wAppend(&wb, buf, strlen(buf));
    wAppend(&wb, "\x1b[2K", 4);

    if (status == WIN)
        draw_centered_string(&wb, "You win!", y);

    else if (status == LOSS)
        draw_centered_string(&wb, "You lose.", y);

    write(STDOUT_FILENO, wb.b, wb.len);
    exit(0);
}

void draw_win()
{
    draw_endgame(WIN);
}

void draw_loss()
{
    draw_endgame(LOSS);
}

void refresh_screen()
{
    struct wbuf wb = WBUF_INIT;
    wAppend(&wb, "\x1b[2J", 4);
    wAppend(&wb, "\x1b[H", 3);

    draw_information(&wb);
    draw_map(&wb);

    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.ay + E.cy, E.ax + E.cx);
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
        if (E.cy < board->size - 1)
            E.cy++;
        break;
    case 'd':
        if (E.cx < board->size - 1)
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
        enum GameState state = reveal_tile(E.cx, E.cy);
        if (state == LOSS)
        {
            draw_loss();
            return;
        }
        break;
    case 'f':
        place_flag(E.cx, E.cy);
        break;
    case 'q':
        exit(0);
        break;
    }
}