#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>
#include "input.h"
#include "command_window.h"
#include "track_mouse.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "globals.h"
#include <fcntl.h>

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

void *mouse_thread(void *vargp) {
  while (true) {
  sleep(0.01f);
  unsigned long int current_win_num;
  get_window_num(&current_win_num);
  if (current_win_num!=jute_terminal_window)
    system("xdotool mousedown 1");
  sleep(0.01f);
  system("xdotool mouseup 1");
  }
}

char *input_string;
bool is_old_mouse_mask = false;
bool is_command_command_string_allocated = false;

void free_last_input_string() {
  free(input_string);
}

char* mvwget_input_string_with_mouse(WINDOW *win, const int irow, const int icol, MEVENT *mouse_event, bool *pressed_mouse, bool process_dirs) {
  current_input_col=icol;
  start_reporting_mouse();
  short c = mvwgetch(win, irow, current_input_col);
  short max_input_string_length = max_col - command_col;
  input_string = calloc(max_input_string_length , sizeof(char));
  char *string_builder = input_string;
  while (c!='\n'&&current_input_col!=icol+max_input_string_length) {
    if ((c==KEY_BACKSPACE||c==127))
    {
      if (current_input_col==icol)
      {
        c = wgetch(win);
        continue;
      }
      string_builder--;
      current_input_col--;
      mvwdelch(win, irow, current_input_col);
      wrefresh(win);
    }
    else if (isalpha(c)||c==' '||c=='-'||c=='.'||c=='!'||c==',')
    {
      waddch(win, c);
      *string_builder++ = c;
      current_input_col++;
    }
      else if (c == KEY_MOUSE) {
        if (getmouse(mouse_event) == OK) {
          if (((*mouse_event).bstate & BUTTON1_CLICKED)||((*mouse_event).bstate & BUTTON1_PRESSED)||((*mouse_event).bstate & BUTTON1_RELEASED))
            *pressed_mouse = true;
          else
            *pressed_mouse = false;
        }
        if (process_dirs)
          update_dirs_mouse_input(mouse_event, *pressed_mouse);
    }
    wrefresh(win);
    c = mvwgetch(win, irow, current_input_col);
  }
  stop_reporting_mouse();
  *(++string_builder)='\0';
  return input_string;
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
