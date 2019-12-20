#ifndef COMMAND_WINDOW_H
#define COMMAND_WINDOW_H
#define tabs_row 3
#include <ncurses.h>
#include "dirs.h"
#include "globals.h"
#define self command_window

typedef struct _cmdtext cmdtext;
struct _cmdtext {
    char *cmd_name;
    int tcmd_row;
    int tcmd_col_from;
    int tcmd_col_to;
} *commands;
enum command {cmd_none=-1, cmd_goto=0, cmd_cd=1, cmd_show=2, cmd_write=3, cmd_mkdir=4};
enum command current_command;
int number_of_commands;

mmask_t old_mouse_mask;

int dirs_first_col, dirs_first_row, dirs_max_row, dirs_max_col;

int current_input_col;
int command_input_row, command_input_col;
short max_input_string_length;
char *input_string, *string_builder;

bool is_hinted;
bool backspaced_whenhint;

int cmdshorts_col, cmd_shorts_max_col;
char* cmdshorts_text;

void init_command_window();

void init_command_names();

void show_command_window();

void print_cmdw_space();

void print_cmdw_title(char *title);

void print_dirs_space();

void show_dirs();

void update_dirs_with_mouse_input(MEVENT *mouse_event, bool pressed_mouse);

void input_command();

char* mvwget_input_string_with_mouse(WINDOW *win, const int irow, const int icol, MEVENT *mouse_event, bool *pressed_mouse, bool process_dirs);

void start_reporting_mouse ();
void stop_reporting_mouse ();

void check_input_string();

void free_last_input_string();

void clear_command_line();

void print_cmdshorts_title();

void print_shortcuts_list();

void print_border();

#endif
