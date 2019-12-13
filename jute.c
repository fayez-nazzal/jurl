#include <ncurses.h>
#include <stdio.h>
#include "jute.h"
#include "globals.h"
#include "dirs.h"
#include "command_window.h"
#include "date_time.h"
#include "input.h"
#include "main_window.h"
#include "colors.h"

int main() {
    /* print the title */
    printf("\033]0; Jute \007");

    initscr();
    raw();
    noecho();
    getmaxyx(stdscr, number_of_rows, number_of_cols);
    while (number_of_cols<56||number_of_rows<16) {
        mvwprintw(stdscr, 0, 0, "Your terminal size is very small, please resize it");
        refresh();
        delay(0.1f);
        getmaxyx(stdscr, number_of_rows, number_of_cols);
    }
    init_colors();

    init_main_window();
    init_command_window();

    if (has_colors() && can_change_color()) {
        show_command_window();
        command_window_show_directories();
        int c;
        while ((c = getch()) != 'x');
    } else {
        for (int i = 3; i > 0; i--) {
            mvwprintw(stdscr, 0, 0, "Your terminal does not support colors\nExiting after %i seconds", i);
            wrefresh(stdscr);
            delay(1);
        }
    }
    endwin();
    return 0;
}
