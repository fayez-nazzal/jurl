#ifndef GLOBALS_H
#define GLOBALS_H

#include <ncurses.h>

#define MODERN_WHITE  1
#define MODERN_BLACK  2
#define GREY_WHITE  3
#define MODERN_GREEN  4
#define MODERN_YELLOW  5
WINDOW *main_window;
WINDOW *command_window;
int number_of_rows, number_of_cols;
bool can_input;
#endif
