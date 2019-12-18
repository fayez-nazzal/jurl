#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "dirs.h"
#include "command_window.h"
#include "date_time.h"

void init_command_window() {
    command_window = newwin(number_of_rows, number_of_cols, 0, 0);
    tab1_selected = tab2_selected = false;
    update_date_time();
    curr_cw_month = month;
    curr_cw_year = year;
    cal_mthdays_allocated=false;
    init_command_names();
    max_col = number_of_cols - 6;
    max_row = number_of_rows / 2 + (number_of_rows/8) - 2;
    command_row = max_row;
    command_col = number_of_cols - (number_of_cols / 2 + number_of_cols/8);
    first_col = command_col;
    first_row = 4;
    tab1_col = 1;
    tab2_col = tab1_col+11;
    tabs_max_col = tab1_col + 22;
    tab1_text = " commands";
    tab2_text = " calender ";
    keypad(command_window, TRUE);
    wrefresh(command_window);
}

void init_command_names() {
    commands = calloc(20, sizeof(struct cmd_text));
    commands[0].cmd_name = "go-to []";
    commands[1].cmd_name = "make-dir []";
    commands[2].cmd_name = "show []";
    commands[3].cmd_name = "remove []";
    commands[4].cmd_name = "encrypt";
    commands[5].cmd_name = "sync";
    commands[6].cmd_name = "docs";
    commands[7].cmd_name = "bored";
    commands[8].cmd_name = "todo";
    commands[9].cmd_name = "export [d]";
    commands[10].cmd_name = "sort [s]";
    number_of_commands = 11;
}

void show_command_window() {
    attron(COLOR_PAIR(1));
    print_tabs_space();
    print_tabs_titles();
    select_tab_1();
    print_border();
    get_directories(".");
    command_window_show_directories();
    input_command();
    attroff(COLOR_PAIR(1));
    wrefresh(command_window);
}

void print_tabs_space() {
    static const int tab_titles_row = tabs_row;
    for (int i = tab1_col; i < tabs_max_col-1; i++) {
        for (int j = tab_titles_row; j < number_of_rows; j++)
        {
            if (j==tab_titles_row)
            {
                if (i==tab2_col-1)
                {
                    wattron(command_window, COLOR_PAIR(2));
                    mvwprintw(command_window, j, i, "%c", ' ');
                    wattroff(command_window, COLOR_PAIR(2));
                }
                else
                {
                    wattron(command_window, COLOR_PAIR(1));
                    mvwprintw(command_window, j, i, "%c", ' ');
                    wattroff(command_window, COLOR_PAIR(1));
                }
            }
            else /* create a space between tab1 and tab2 titles */
            {
                wattron(command_window, COLOR_PAIR(2));
                mvwprintw(command_window, j, i, "%c", ' ');
                wattroff(command_window, COLOR_PAIR(2));
            }
        }
    }
}

void print_cmdw_title(char *title) {
    mvwprintw(command_window, 1, number_of_cols / 2 - strlen(title) / 2, title);
}

void print_tabs_titles() {
    int temp_first_row = tabs_row;
    wattron(command_window, COLOR_PAIR(1));
    mvwprintw(command_window, temp_first_row, tab1_col, tab1_text);
    mvwprintw(command_window, temp_first_row, tab2_col, tab2_text);
    wattroff(command_window, COLOR_PAIR(1));
    wrefresh(command_window);
}

void print_border() {
    for (int i = 1, j = number_of_rows - 2; i < number_of_rows - 1 && j > 0; i++, j--) {
        mvwaddch(command_window, i, 0, '|');
        mvwaddch(command_window, j, number_of_cols - 1, '|');
    }
    for (int i = 1, j = number_of_cols - 2; i < number_of_cols - 1 && j > 0; i++, j--) {
        mvwaddch(command_window, 1, i, '-');
        mvwaddch(command_window, number_of_rows - 1, j, '-');
    }
    mvwaddch(command_window, 1, 0, '+');
    mvwaddch(command_window, number_of_rows - 1, 0, '+');
    mvwaddch(command_window, 1, number_of_cols - 1, '+');
    mvwaddch(command_window, number_of_rows - 1, number_of_cols - 1, '+');
    print_cmdw_title(" >>Command Window<< ");
}

void print_dirs_space() {
    for (int i = first_col; i < max_col; i++) {
        for (int j = first_row; j < max_row; j++)
            mvwaddch(command_window, j, i, ' ' | COLOR_PAIR(2));
    }
}

void select_tab_1() {
    tab2_selected=false;
    tab1_selected=true;
    // wattron(command_window, COLOR_PAIR(2));
    // mvwprintw(command_window, tabs_row+1, tab1_col, "                 ");
    // wattroff(command_window, COLOR_PAIR(2));
    print_tabs_space();
    print_tabs_titles();
    print_border();
    wattron(command_window, COLOR_PAIR(2));
    for (int i=tab1_col; i<tab2_col-1; i++)
        mvwprintw(command_window, tabs_row, i, " ");
    mvwprintw(command_window, tabs_row, tab1_col, "%s", tab1_text);
    wrefresh(command_window);
    wattroff(command_window, COLOR_PAIR(2));
    print_shortcuts_list();
}

void print_shortcuts_list() {
    wattron(command_window, COLOR_PAIR(2));
    for (int i=0; i<number_of_commands; i++) {
        mvwprintw(command_window, tabs_row+1+i, tab1_col, commands[i].cmd_name);
        commands[i].tcmd_row = tabs_row+1+i;
        commands[i].tcmd_col_from = tab1_col;
        commands[i].tcmd_col_to = tab1_col+strlen(commands[i].cmd_name);
    }
    wattroff(command_window, COLOR_PAIR(2));
}

void select_tab_2(int m) {
    tab1_selected=false;
    tab2_selected=true;
    print_tabs_space();
    print_tabs_titles();
    // wattron(command_window, COLOR_PAIR(2));
    // for (int i=tab2_col+1; i<number_of_cols-1; i++)
    //     mvwprintw(command_window, tabs_row, i, " ");
    // wattroff(command_window, COLOR_PAIR(2));
    print_border();
    wattron(command_window, COLOR_PAIR(2));
    mvwprintw(command_window, tabs_row, tab2_col, "%s", tab2_text);
    wrefresh(command_window);
    wattroff(command_window, COLOR_PAIR(2));
    update_date_time();
    wrefresh(command_window);
}

void print_calender(int m) {
    if (cal_mthdays_allocated) {
        free(cal_mthdays);
        cal_mthdays_allocated=false;
    }
    static char* months[] = {
        "January ", "February ", "March ", "April ", "May ", "June ", "July ", "August ", "September ", "October ", "November ", "December "
    };
    char *month_with_year = calloc(20, sizeof(char));
    strcpy(month_with_year, months[m-1]);
    char yearstr[4];
    sprintf(yearstr, "%d", curr_cw_year);
    strcat(month_with_year, yearstr);
    int days_number = mthdays(m, year);
    int available_space = number_of_cols - tab1_col;
    int month_len = strlen(month_with_year);
    int month_print_pos = available_space/2 - month_len/2;
    cal_mthdays = calloc(days_number, sizeof(struct calender_mthday));

    wattron(command_window, COLOR_PAIR(2));
    mvwprintw(command_window, tabs_row+1, tab1_col+month_print_pos, "%s", month_with_year);
    prev_month_row = next_month_row = tabs_row+1;
    prev_month_col = tab1_col+month_print_pos-2;
    next_month_col = prev_month_col + month_len+2;
    mvwprintw(command_window, prev_month_row, prev_month_col, "<<");
    mvwprintw(command_window, next_month_row, next_month_col, ">>");
    mvwprintw(command_window, tabs_row+2, tab1_col, "S  M  T  W  T  F  S");

    curr_cal_mthdays_len = days_number;

    const int first_weekday_of_month = dayofweek(1, m, year);
    int day_print_position = first_weekday_of_month*3;
    int weekday=0;
    int print_row=tabs_row+3;
    for (int i=1; i<=days_number; i++)
    {
        weekday=dayofweek(i, m, year);
        int printcol = tab1_col+(weekday*3);
        wattron(command_window, COLOR_PAIR(2));
        if (m==month&&curr_cw_year==year&&i==day)
            wattron(command_window, COLOR_PAIR(3));
        mvwprintw(command_window, print_row, printcol, "%02d", i);
        wattroff(command_window, COLOR_PAIR(3));
        wattroff(command_window, COLOR_PAIR(2));

        cal_mthdays[i].daynum = i;
        cal_mthdays[i].col_f = printcol;
        cal_mthdays[i].col_t = printcol+2;
        cal_mthdays[i].row = print_row;

        if (m==month&&curr_cw_year==year&&i==day)
            wattroff(command_window, COLOR_PAIR(3));
        if (weekday==6) {
            day_print_position=0;
            weekday=0;
            print_row++;
        }
    }
    wmove(command_window, command_row, current_input_col);
    wattroff(command_window, COLOR_PAIR(2));
    wrefresh(command_window);
    free(month_with_year);
    cal_mthdays_allocated=true;
}

void input_command() {
    wrefresh(command_window);
    MEVENT mouse_event;
    bool pressed_mouse = false;
    char *string = mvwget_input_string_with_mouse(command_window, command_row, command_col, &mouse_event,
                                                  &pressed_mouse, true);
    wrefresh(command_window);
}

/* TODO recognise rest of input types */
void check_input_string() {
    char *p;
    if (((p=strchr(input_string, '[')) != NULL)&&(*(p+2)==']')) {
        char identifier = *(p+1);
        char *helpstring;
        is_hint=true;
        int tempcol = current_input_col;
        char *backup_string = calloc(40, sizeof(char));
        int toindex = p-input_string;
        strncpy(backup_string, input_string, toindex);
        free_last_input_string();
        input_string=backup_string;
        string_builder=input_string+toindex;
        wmove(command_window, command_row, tempcol);
        current_input_col-=3;
        switch (identifier)
        {
            case 'd':
                helpstring = "Enter a directory";
        }
        wattron(command_window,COLOR_PAIR(3));
        mvwprintw(command_window, command_row, current_input_col, "%s", helpstring);
        wattroff(command_window,COLOR_PAIR(3));
        wrefresh(command_window);
    }
}

void command_window_show_directories() {
    /* draw directories window */
    print_dirs_space();

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
}

/* TODO make a 2D array of a struct (day) type, struct day days[x][y] an x is added in case clicked (>>),
        there's a variables identifieng x_count, y_count */
/* TODO add a loop breaker for each loop to improve performance */
void update_dirs_mouse_input(MEVENT *mouse_event, bool pressed_mouse) {
    int m_row = (*mouse_event).y, m_col = (*mouse_event).x;
    for (int i = 0; i < number_of_dirs; i++) {
        if (directories[i].dir_row == m_row &&
            (directories[i].dir_col_from <= m_col && directories[i].dir_col_to >= m_col)) {
            wattron(command_window, COLOR_PAIR(1));
            mvwprintw(command_window, directories[i].dir_row, directories[i].dir_col_from, "%s",
                      directories[i].dir_name);
            wmove(command_window, command_row, current_input_col);
            wattroff(command_window, COLOR_PAIR(1));
            if (pressed_mouse)
                {
                    get_directories(directories[i].dir_name);
                    command_window_show_directories();
                }
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
    if (tab1_selected) {
        for (int i=0; i<number_of_commands; i++)
            if (commands[i].tcmd_row == m_row &&
                (commands[i].tcmd_col_from <= m_col && commands[i].tcmd_col_to >= m_col)) {
                    wattron(command_window, COLOR_PAIR(1));
                    mvwprintw(command_window, commands[i].tcmd_row, commands[i].tcmd_col_from, "%s",
                        commands[i].cmd_name);
                    wmove(command_window, command_row, current_input_col);
                    wattroff(command_window, COLOR_PAIR(1));
                    wrefresh(command_window);
                    if (pressed_mouse)
                    {
                        int len = strlen(commands[i].cmd_name);
                        mvwprintw(command_window, command_row, current_input_col, "%s", commands[i].cmd_name);
                        strcpy(input_string, commands[i].cmd_name);
                        wmove(command_window, command_row, current_input_col+=len);
                        check_input_string();
                    }          
                } else {
                    wattron(command_window, COLOR_PAIR(2));
                    mvwprintw(command_window, commands[i].tcmd_row, commands[i].tcmd_col_from, "%s",
                        commands[i].cmd_name);
                    wmove(command_window, command_row, current_input_col);
                wattroff(command_window, COLOR_PAIR(2));
                wrefresh(command_window);
                }
    }

    if (!tab1_selected) {
        if (m_row == tabs_row && m_col > tab1_col && m_col < tab2_col)
            {
                    wattron(command_window, COLOR_PAIR(3));
                    mvwprintw(command_window, tabs_row, tab1_col, "%s",
                        tab1_text);
                    mvwprintw(command_window, tabs_row, tab1_col+strlen(tab1_text), "%s",
                        " ");
                    wattroff(command_window, COLOR_PAIR(3));
                    wrefresh(command_window); 
                    if (pressed_mouse) {
                        print_tabs_space();
                        print_tabs_titles();
                        select_tab_1();
                    }
                    wmove(command_window, command_row, current_input_col);
            } else {
                    wattron(command_window, COLOR_PAIR(1));
                    mvwprintw(command_window, tabs_row, tab1_col, "%s",
                        tab1_text);
                    mvwprintw(command_window, tabs_row, tab1_col+strlen(tab1_text), "%s",
                        " ");
                    wmove(command_window, command_row, current_input_col);
                    wattroff(command_window, COLOR_PAIR(1));
                    wrefresh(command_window); 
        }
    }
    if (!tab2_selected)
    {
        if (m_row == tabs_row && m_col > tab2_col && m_col < tabs_max_col)
            {
                wattron(command_window, COLOR_PAIR(3));
                mvwprintw(command_window, tabs_row, tab2_col, "%s",
                        tab2_text);
                wattroff(command_window, COLOR_PAIR(3));
                wrefresh(command_window);
                if (pressed_mouse)
                {
                    select_tab_2(month);
                }
                wmove(command_window, command_row, current_input_col);
            } else {
            wattron(command_window, COLOR_PAIR(1));
                mvwprintw(command_window, tabs_row, tab2_col, "%s",
                        tab2_text);
                    wmove(command_window, command_row, current_input_col);
                    wattroff(command_window, COLOR_PAIR(1));
                    wrefresh(command_window); 
        }
    }
}

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
  is_hint = backspaced_whenhint = false;
  free_last_input_string();
  input_string = calloc(max_input_string_length , sizeof(char));
  string_builder = input_string;
  while (c!='\n'&&current_input_col!=icol+max_input_string_length-20) { /* -20 is for check_string like [d] */
    if ((c==KEY_BACKSPACE||c==127))
    {
      if (!is_hint && !backspaced_whenhint) {
        if (current_input_col==icol)
        {
          c = wgetch(win);
          continue;
        }
        string_builder--;
        current_input_col--;
        mvwprintw(command_window, irow, current_input_col, " ");
        wrefresh(win);
        wattroff(command_window,COLOR_PAIR(2));
      }
      else if (backspaced_whenhint) {
        for (int i=first_col; i<current_input_col + max_input_string_length; i++) {
        mvwprintw(command_window, irow, i, " ");
        wmove(command_window, command_row, first_col); }
        backspaced_whenhint=false;
        wrefresh(win);
        is_hint = false;
        free_last_input_string();
        input_string = calloc(max_input_string_length , sizeof(char));
        current_input_col=first_col;
        string_builder = input_string;
        continue;
      } else {
        for (int i=current_input_col; i<current_input_col + max_input_string_length; i++) {
        mvwprintw(command_window, irow, i, " ");
        wmove(command_window, command_row, current_input_col);
        }
        is_hint=false;
        wrefresh(win);
        backspaced_whenhint=true;
      }
    }
    if (isalpha(c)||isdigit(c)||isspace(c)||isxdigit(c)||ispunct(c))
    {
      if (is_hint)
        for (int i=current_input_col; i< current_input_col + max_input_string_length; i++) {
          mvwprintw(command_window, irow, i, " ");
          wmove(command_window, command_row, current_input_col);
        }
      backspaced_whenhint=false;
      mvwprintw(win, command_row, current_input_col, "%c", c);
      *string_builder = c;
      string_builder++;
      current_input_col++;
      is_hint=false;
      wrefresh(win);
      check_input_string();
    }
      else if (c == KEY_MOUSE) {
        if (getmouse(mouse_event) == OK) {
          if (((*mouse_event).bstate & BUTTON1_CLICKED)||((*mouse_event).bstate & BUTTON1_PRESSED)||((*mouse_event).bstate & BUTTON1_RELEASED)||((*mouse_event).bstate & BUTTON1_DOUBLE_CLICKED))
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
  *(++string_builder)='\0';
  return input_string;
}

void stop_reporting_mouse () {
  mousemask(0, &old_mouse_mask);
  //printf("\033[?1003l\n");
}

void start_reporting_mouse () {
  if (is_old_mouse_mask)
    old_mouse_mask = mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, &old_mouse_mask);
  else
    old_mouse_mask = mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
  printf("\033[?1003h\n");
}