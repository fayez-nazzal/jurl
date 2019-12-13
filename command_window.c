#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dirs.h"
#include "command_window.h"
#include "input.h"
#include "date_time.h"

void init_command_window() {
    command_window = newwin(number_of_rows, number_of_cols, 0, 0);
    max_col = number_of_cols / 2 + number_of_cols/8;
    max_row = number_of_rows / 2 + (number_of_rows/8);
    command_row = max_row;
    command_col = 5;
    keypad(command_window, TRUE);
    wrefresh(command_window);
}

void show_command_window() {
    //attron(COLOR_PAIR(1));
    //print_border(command_window, number_of_rows, col);
    //print_title(command_window, col, "Command Window");
    //attroff(COLOR_PAIR(1));
    wrefresh(command_window);
}

void command_window_show_directories() {
    /* draw directories window */
    get_directories();
    const int first_col = 5;
    const int first_row = 3;
    for (int i = first_col; i < max_col; i++) {
        for (int j = first_row; j < max_row; j++)
            mvwaddch(command_window, j, i, ' ' | COLOR_PAIR(2));
    }
    int current_row = first_row, current_col = first_col;
    int dir_col_f;
    for (int i = 0; i < number_of_dirs; i++) {
        char *dir_name_nspace = directories[i].dir_name;
        char *dir_name;
        int dir_length = strlen(dir_name_nspace);
        if (max_col - current_col < dir_length + 2) {
            current_col = first_col;
            dir_col_f = first_col;
            current_row += 1;
            dir_name = dir_name_nspace;
        } else {
            dir_name = calloc(dir_length + 2, sizeof(char));
            if (current_col != first_col) {
                strcpy(dir_name, "  ");
                dir_col_f = current_col + 2;
            } else
                dir_col_f = current_col;
            strcat(dir_name, dir_name_nspace);
            dir_length += 2;
        }
        directories[i].dir_col_from = dir_col_f;
        while (*dir_name != '\0') {
            mvwaddch(command_window, current_row, current_col++, *dir_name | COLOR_PAIR(2));
            dir_name++;
        }
        directories[i].dir_col_to = current_col - 1;
        directories[i].dir_row = current_row;

    }
    update_dir_window();
}

void update_dir_window() {
    start_reporting_mouse();
    wrefresh(command_window);
    MEVENT mouse_event;
    bool pressed_mouse = false;
    char *string = mvwget_input_string_with_mouse(command_window, command_row, command_col, &mouse_event,
                                                  &pressed_mouse, true);
    wrefresh(command_window);
    free_last_input_string();
}

void update_dirs_mouse_input(MEVENT *mouse_event, bool pressed_mouse) {
    int m_row = (*mouse_event).y, m_col = (*mouse_event).x;
    for (int i = 0; i < number_of_dirs; i++)
        if (directories[i].dir_row == m_row &&
            (directories[i].dir_col_from <= m_col && directories[i].dir_col_to >= m_col)) {
            wattron(command_window, COLOR_PAIR(1));
            mvwprintw(command_window, directories[i].dir_row, directories[i].dir_col_from, "%s",
                      directories[i].dir_name);
            wmove(command_window, command_row, current_input_col);
            wattroff(command_window, COLOR_PAIR(1));
            delay(0.01f);
            wrefresh(command_window);
        } else {
            wattron(command_window, COLOR_PAIR(2));
            mvwprintw(command_window, directories[i].dir_row, directories[i].dir_col_from, "%s",
                      directories[i].dir_name);
            wmove(command_window, command_row, current_input_col);
            wattroff(command_window, COLOR_PAIR(2));
            wrefresh(command_window);
        }
}


void print_border() {
    mvwaddch(command_window, 0, 0, '+');
    mvwaddch(command_window, number_of_rows - 1, 0, '+');
    mvwaddch(command_window, 0, number_of_cols - 1, '+');
    mvwaddch(command_window, number_of_rows - 1, number_of_cols - 1, '+');
    for (int i = 1, j = number_of_rows - 2; i < number_of_rows - 1 && j > 0; i++, j--) {
        mvwaddch(command_window, i, 0, '|');
        mvwaddch(command_window, j, number_of_cols - 1, '|');
    }
    for (int i = 1, j = number_of_cols - 2; i < number_of_cols - 1 && j > 0; i++, j--) {
        mvwaddch(command_window, 0, i, '-');
        mvwaddch(command_window, number_of_rows - 1, j, '-');
    }
}

void print_title(char *title) {
    mvwprintw(command_window, 1, number_of_cols / 2 - strlen(title) / 2, title);
}
