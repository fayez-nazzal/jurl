#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dirs.h"
#include "command_window.h"
#include "input.h"

void command_window_show_directories() {
  /* draw directories window */
  const int first_col = col/4;
  const int first_row = row/4;
  const int max_col = col/2 + col/4;
  const int max_row = row/2 + row/4;
  for (int i=first_col; i<max_col; i++)
  {
    for (int j=first_row; j< max_row; j++)
      mvwaddch(command_window, j, i, ' ' | COLOR_PAIR(2));
  }
  int current_row = first_row, current_col=first_col;
  int dir_col_f;
  for (int i=0; i< number_of_dirs; i++)
  {
    char *dir_name_nspace = directories[i].dir_name;
    char *dir_name;
    int dir_length = strlen(dir_name_nspace);
    if (max_col - current_col < dir_length+2)
    {
      current_col = first_col;
      current_row++;
      dir_name = dir_name_nspace;
    }
    else
    {
      dir_name = calloc(dir_length+2, sizeof(char));
      if (current_col!=first_col)
      {
        strcpy(dir_name, "  ");
        dir_col_f = current_col+2;
      }
      else
      dir_col_f = current_col;
      strcat(dir_name, dir_name_nspace);
      dir_length+=2;
    }
    directories[i].dir_col_from = dir_col_f;
    directories[i].dir_row = current_row - 1;
    while (*dir_name!='\0') {
      mvwaddch(command_window, current_row, current_col++, *dir_name | COLOR_PAIR(2));
      dir_name++;
    }
    directories[i].dir_col_to = current_col-1;
}
  int rrw = row/4 + row/2 + 1;
  for (int i=0; i<number_of_dirs; i++)
    mvwprintw(command_window, rrw++, 0, "dir name: %s, dir row: %i dir col from: %i dir col to: %i", directories[i].dir_name, directories[i].dir_row, directories[i].dir_col_from, directories[i].dir_col_to);
   update_dir_window();
}

void update_dir_window() {
  start_reporting_mouse();
  wrefresh(command_window);
  MEVENT mouse_event;
  bool pressed_mouse = false;
  int command_row = row/4 + row/2;
  char *s = mvwget_input_string_with_mouse(command_window, command_row, col/4, &mouse_event, &pressed_mouse, true);

  wrefresh(command_window);
}

void update_dirs_mouse_input(MEVENT *mouse_event, bool pressed_mouse) {
  int m_row = (*mouse_event).y, m_col = (*mouse_event).x;
  if (pressed_mouse)
  for (int i=0; i<number_of_dirs; i++)
    if(directories[i].dir_row==m_row&&(directories[i].dir_col_from<=m_col&&directories[i].dir_col_to>=m_col))
     {
       mvwprintw(command_window, row/4 + row/2, col/4 + col/2, "row: %i, col: %i, name: %s",m_row, m_col, directories[i].dir_name);
       //mvwaddch(command_window, row/4 + row/2, col/4 + col/2, 's');
     }
}
