#include <ncurses.h>
#include "globals.h"

void init_colors() {
  start_color();
  init_color (MODERN_BLACK, 56, 57, 58);
  init_color (MODERN_WHITE, 790, 820, 810);
  init_pair (1, MODERN_BLACK, MODERN_WHITE);
  init_color (GREY_WHITE, 290, 329, 310);
  init_color (MODERN_GREEN, 215, 790, 119);
  init_pair (2, COLOR_WHITE, GREY_WHITE);
}
