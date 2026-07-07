#ifndef READER_H
#define READER_H

void disable_raw_mode();
void enable_raw_mode();
void init_terminal();
void init_game();
void process_key();

void refresh_screen();
void clear_screen();

void draw_loss();
void draw_win();
void draw_menu();

#endif