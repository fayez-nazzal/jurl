#ifndef COMMAND_WINDOW_H
#define COMMAND_WINDOW_H
#define tabs_row 3
#include <ncurses.h>
#include "dirs.h"
#include "globals.h"

struct cmd_text {
    char *cmd_name;
    int tcmd_row;
    int tcmd_col_from;
    int tcmd_col_to;
};
struct cmd_text *commands;

struct calender_mthday {
    int daynum, row, col_f, col_t;
};
struct calender_mthday *cal_mthdays;
int curr_cal_mthdays_len;
bool cal_mthdays_allocated;
int curr_cw_month, curr_cw_year;
char prev_month, next_month;
int prev_month_row, prev_month_col, next_month_row, next_month_col;
int number_of_commands;
char* tab1_text, *tab2_text;
bool tab1_selected, tab2_selected;
int command_row, command_col;
int first_col, first_row;
int tab1_col, tab2_col, tabs_max_col;
int max_row, max_col;
mmask_t old_mouse_mask;
int current_input_col;
char *input_string, *string_builder;
bool is_hint;
bool backspaced_whenhint;
void free_last_input_string();
char* mvwget_input_string_with_mouse(WINDOW *win, const int irow, const int icol, MEVENT *mouse_event, bool *pressed_mouse, bool process_dirs);
void stop_reporting_mouse ();
void start_reporting_mouse ();
void *mouse_thread(void *vargp);

void init_command_window();

void init_command_names();

void show_command_window();

void print_tabs_space();

void print_cmdw_title(char *title);

void print_tabs_titles();

void print_border();

void print_dirs_space();

void select_tab_1();

void print_shortcuts_list();

void select_tab_2(int m);

void print_calender();

//void print karma();

//void print todo();
char *strremove(char *str, const char *sub);

void input_command();

void check_input_string();

void command_window_show_directories();

void update_dirs_mouse_input(MEVENT *mouse_event, bool pressed_mouse);















#endif
