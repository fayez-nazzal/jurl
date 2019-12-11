#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void delay(int seconds);
void readfile (FILE *fp, char *fi);
void show_command_window();
void get_command();
void print_border(WINDOW *win, int num_rows, int num_columns);
void print_title(WINDOW *win, int num_cols, char *title);
