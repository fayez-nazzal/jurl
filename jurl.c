#include <ncurses.h>
#include <stdio.h>
#include "jurl.h"
#include "globals.h"
#include "dirs.h"
#include "command_window.h"
#include "date_time.h"
#include "input.h"
#include "main_window.h"
#include "colors.h"

// TODO make program not work with very small size terminals
// TODO change name to juary
int main()
{
  /* print the title */
 printf("\033]0; Jurl \007");
 initscr();
 raw();
 noecho();
 keypad(stdscr, TRUE);
 getmaxyx(stdscr,number_of_rows,number_of_cols);
 init_colors();
 init_main_window();
 init_command_window();
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
 show_command_window();
 command_window_show_directories();
 int c;
 while ((c=getch())!='x');
 endwin();
 return 0;
}
