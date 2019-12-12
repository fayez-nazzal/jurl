#include <ncurses.h>
#include "globals.h"

void init_main_window() {
  main_window = newwin(number_of_rows, number_of_cols, 0, 0);
  keypad(main_window, TRUE);
  wbkgd(main_window, COLOR_PAIR(1));
}
