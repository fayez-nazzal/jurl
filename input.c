#include <ncurses.h>
#include <stdlib.h>
#include "input.h"
#include "command_window.h"
bool is_old_mouse_mask = false;

/* Mouse clicks sent through bstate */
int mvwget_input_with_mouse(WINDOW *win, int row, int col, MEVENT *mouse_event) {
  start_reporting_mouse();
  move(row, col);
  while (true) {
    int c = wgetch(win);
    if (c == KEY_MOUSE) {
      MEVENT event;
      if (getmouse(&event) == OK) {
        *mouse_event = event;
        stop_reporting_mouse();
        return c;
      }
    }
    else {
      stop_reporting_mouse();
      return c;
    }
  }
  stop_reporting_mouse();
  return -1;
}

int mvwget_input(WINDOW *win, int row, int col) {
  move(row, col);
  return wgetch(win);
}

char* mvwget_input_string(WINDOW *win, const int irow, const int icol) {
}

char* mvwget_input_string_with_mouse(WINDOW *win, const int irow, const int icol, MEVENT *mouse_event, bool *pressed_mouse, bool process_dirs) {
  int current_col=icol;
  start_reporting_mouse();
  int c = mvwgetch(win, irow, current_col);
  char *string = calloc(200, sizeof(char));
  char *string_maker = string;
  while (c!='\n'&&current_col!=icol+30) {
      if (c == KEY_MOUSE) {
        if (getmouse(mouse_event) == OK) {
          if (((*mouse_event).bstate & BUTTON1_CLICKED)||((*mouse_event).bstate & BUTTON1_PRESSED)||((*mouse_event).bstate & BUTTON1_RELEASED))
            *pressed_mouse = true;
          else
            *pressed_mouse = false;
        }
        if (process_dirs)
          update_dirs_mouse_input(mouse_event, *pressed_mouse);
    }
    if ((c==KEY_BACKSPACE||c==127))
    {
      if (current_col==icol)
      {
        c = wgetch(win);
        continue;
      }
      string_maker--;
      current_col--;
      mvwdelch(win, irow, current_col);
      wrefresh(win);
    }
    else if (isalpha(c)||c=='-'||c=='.')
    {
      waddch(win, c);
      *string_maker++ = c;
      current_col++;
    }
    wrefresh(win);
    c = mvwgetch(win, irow, current_col);
  }
  stop_reporting_mouse();
  *(++string_maker)='\0';
}

void stop_reporting_mouse () {
  mousemask(0, &old_mouse_mask);
  printf("\033[?1003l\n");
}

void start_reporting_mouse () {
  if (is_old_mouse_mask)
    old_mouse_mask = mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, &old_mouse_mask);
  else
    old_mouse_mask = mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
  printf("\033[?1003h\n");
}
