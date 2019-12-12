#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include "jurl.h"
#include "globals.h"
#include "dirs.h"
#include "command_window.h"
#include "input.h"

// TODO make program not work with very small size terminals
int main()
{
  /* print the title */
 printf("\033]0; Jurl \007");
 initscr();
 raw();
 keypad(stdscr, TRUE);
 noecho();
 getmaxyx(stdscr,row,col);
 main_window = newwin(row, col, 0, 0);
 keypad(main_window, TRUE);
 command_window = newwin(row, col, 0, 0);
 init_command_window();
 keypad(command_window, TRUE);
 refresh();
 if (has_colors() == FALSE || !can_change_color())
 {
   for (int i=3; i>0; i--) {
   mvwprintw(stdscr, 0, 0, "Your terminal does not support colors\nExiting after %i seconds", i);
   wrefresh(stdscr);
   delay(1);
   clear();
   refresh();
   }
   exit(1);
   return 0;
 }

 /* color setup */
 start_color();
 init_color (MODERN_BLACK, 56, 57, 58);
 init_color (MODERN_WHITE, 790, 820, 810);
 init_pair (1, MODERN_BLACK, MODERN_WHITE);
 init_color (GREY_WHITE, 290, 329, 310);
 init_color (MODERN_GREEN, 215, 790, 119);
 init_pair (2, COLOR_WHITE, GREY_WHITE);

 wbkgd(main_window, COLOR_PAIR(1));
 //wbkgd(command_window, COLOR_PAIR(1));
 show_command_window();
 get_directories();
 command_window_show_directories();
 int c;
 while ((c=getch())!='x');
 endwin();
 return 0;
}

void show_command_window() {
  //attron(COLOR_PAIR(1));
  //print_border(command_window, row, col);
  //print_title(command_window, col, "Command Window");
  //attroff(COLOR_PAIR(1));
  wrefresh(command_window);
}

void readfile (FILE *fp, char *fi) {
  while ((*fi++=getc(fp))!=EOF);
}

void print_border(WINDOW *win, int num_rows, int num_columns) {
  mvwaddch(win, 0,0,'+');
  mvwaddch(win, num_rows-1,0,'+');
  mvwaddch(win,0, num_columns-1,'+');
  mvwaddch(win, num_rows-1,num_columns-1,'+');
  for (int i=1, j=num_rows-2; i<num_rows-1 && j>0; i++, j--)
  {
    mvwaddch(win,i,0,'|');
    mvwaddch(win,j,num_columns-1,'|');
  }
  for (int i=1, j=num_columns-2; i<num_columns-1 && j>0; i++, j--)
  {
    mvwaddch(win,0,i,'-');
    mvwaddch(win,num_rows-1,j,'-');
  }
}

void print_title (WINDOW *win, int num_cols, char *title) {
  mvwprintw(win, 1, num_cols/2-strlen(title)/2, title);
}

void get_command() {
  mvwgetch(command_window, 2, 1);
}
