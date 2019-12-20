#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "colors.h"
#include "dirs.h"
#include "command_window.h"
#include "date_time.h"
static int num_spaces=0;
static int last_spaceindex=0;
static char *command_target;
static int command_target_index=0;
static bool backspacedspace, backspaced;
static bool is_old_mouse_mask = false;
static bool is_input_string_allocated = false;

void init_command_window() {
    command_window = newwin(number_of_rows, number_of_cols, 0, 0);
    
    update_date_time();

    dirs_first_row = 4;
    dirs_first_col = number_of_cols - (number_of_cols / 2 + number_of_cols/8);
    dirs_max_row = number_of_rows / 2 + (number_of_rows/8) - 2;
    dirs_max_col = number_of_cols - 6;

    command_input_col = dirs_first_col;
    command_input_row = dirs_max_row;

    cmdshorts_col = 1;
    cmd_shorts_max_col = cmdshorts_col + 22;
    cmdshorts_text = "commands";

    command_target = calloc(30, sizeof(char));
    init_command_names();
    keypad(command_window, TRUE);
    wrefresh(command_window);
}

void init_command_names() {
    commands = calloc(20, sizeof(cmdtext));
    commands[0].cmd_name = "goto [s]";
    commands[1].cmd_name = "show [s]";
    commands[2].cmd_name = "show [f]";
    // commands[3].cmd_name = "remove []";
    // commands[4].cmd_name = "encrypt";
    // commands[5].cmd_name = "sync";
    // commands[6].cmd_name = "docs";
    // commands[7].cmd_name = "bored";
    // commands[8].cmd_name = "todo";
    // commands[9].cmd_name = "export [d]";
    // commands[10].cmd_name = "sort [s]";
    number_of_commands = 3;
}

void show_command_window() {
    current_section = none;
    current_command = -1;
    print_cmdw_space();
    print_cmdshorts_title();
    print_shortcuts_list();
    bool isdir = open_dir(".");
    if (isdir) {
        show_dirs();
        input_command();
    wrefresh(self);
    }
    print_border();
}

void print_cmdw_space() {
    static const int tab_titles_row = tabs_row;
    for (int i = cmdshorts_col; i < cmd_shorts_max_col-1; i++) {
        for (int j = tab_titles_row; j < number_of_rows-1; j++)
        {
            int color_pair_id=1;
            if (j==tab_titles_row)
                color_pair_id=1;
            else /* create a space between tab1 and tab2 titles */
                color_pair_id=2;
            wattron(self, COLOR_PAIR(color_pair_id));
            mvwprintw(self, j, i, "%c", ' ');
            wattroff(self, COLOR_PAIR(color_pair_id));
        }
    }
}

void print_cmdw_title(char *title) {
    mvwprintw(self, 1, number_of_cols / 2 - strlen(title) / 2, title);
}

void print_dirs_space() {
    for (int i = dirs_first_col; i < dirs_max_col; i++) {
        for (int j = dirs_first_row; j < dirs_max_row; j++)
            mvwaddch(self, j, i, ' ' | COLOR_PAIR(2));
    }
}

void show_dirs() {
    /* draw directories window */
    print_dirs_space();

    int current_row = dirs_first_row, current_col = dirs_first_col;
    int dir_col_f;
    for (int i = 0; i < number_of_dirs; i++) {
        char *dir_name_nspace = directories[i].name;
        char *dir_name;
        int dir_length = strlen(dir_name_nspace);
        if (dirs_max_col - current_col < dir_length + 2) {
            current_col = dirs_first_col;
            dir_col_f = dirs_first_col;
            current_row += 1;
            dir_name = dir_name_nspace;
        } else {
            dir_name = calloc(dir_length + 2, sizeof(char));
            if (current_col != dirs_first_col) {
                strcpy(dir_name, "  ");
                dir_col_f = current_col + 2;
            } else
                dir_col_f = current_col;
            strcat(dir_name, dir_name_nspace);
            dir_length += 2;
        }
        directories[i].col_from = dir_col_f;
        while (*dir_name != '\0') {
            mvwaddch(self, current_row, current_col++, *dir_name | COLOR_PAIR(2));
            dir_name++;
        }
        directories[i].col_to = current_col - 1;
        directories[i].row = current_row;

    }
}

void update_dirs_with_mouse_input(MEVENT *mouse_event, bool pressed_mouse) {
    int m_row = (*mouse_event).y, m_col = (*mouse_event).x;
    for (int i = 0; i < number_of_dirs; i++) {
        if (directories[i].row == m_row &&
            (directories[i].col_from <= m_col && directories[i].col_to >= m_col)) {
            wattron(self, COLOR_PAIR(1));
            mvwprintw(self, directories[i].row, directories[i].col_from,
                      directories[i].name);
            wmove(self, command_input_row, current_input_col);
            wattroff(self, COLOR_PAIR(1));
            if (pressed_mouse)
                {
                    bool isdir = open_dir(directories[i].name);
                    show_dirs();
                    if (!isdir)
                        break;
                }
            wrefresh(self);
        } else {
            wattron(self, COLOR_PAIR(2));
            mvwprintw(self, directories[i].row, directories[i].col_from,
                      directories[i].name);
            wmove(self, command_input_row, current_input_col);
            wattroff(self, COLOR_PAIR(2));
            wrefresh(self);
        }
    }
    for (int i=0; i<number_of_commands; i++)
        if (commands[i].tcmd_row == m_row &&
            (commands[i].tcmd_col_from <= m_col && commands[i].tcmd_col_to >= m_col)) {
                wattron(self, COLOR_PAIR(1));
                mvwprintw(self, commands[i].tcmd_row, commands[i].tcmd_col_from, "%s",
                    commands[i].cmd_name);
                wmove(self, command_input_row, current_input_col);
                wattroff(self, COLOR_PAIR(1));
                wrefresh(self);
                if (pressed_mouse)
                {
                    int len = strlen(commands[i].cmd_name);
                    free_last_input_string();
                    input_string = calloc(40, sizeof(char));
                    mvwprintw(self, command_input_row, current_input_col, "%s", commands[i].cmd_name);
                    strcpy(input_string, commands[i].cmd_name);
                    string_builder = input_string+len;
                    wmove(self, command_input_row, current_input_col+=len);
                    check_input_string();
                }          
            } else {
                wattron(self, COLOR_PAIR(2));
                mvwprintw(self, commands[i].tcmd_row, commands[i].tcmd_col_from, "%s",
                    commands[i].cmd_name);
                wmove(self, command_input_row, current_input_col);
            wattroff(self, COLOR_PAIR(2));
            wrefresh(self);
            }
}

void input_command() {
    wrefresh(self);
    MEVENT mouse_event;
    bool pressed_mouse = false;
    mvwget_input_string_with_mouse(self, command_input_row, command_input_col, &mouse_event,
                                    &pressed_mouse, true);
        command_target[command_target_index]='\0';
        mvwprintw(self, 1, 1, "    ");
        mvwprintw(self, 1, 1, command_target);
    if (current_command==cmd_goto)
        {
            if (strcmp(command_target, dirs_get_section_name(0)))
                ; // call section window journal
            else if (strcmp(command_target, dirs_get_section_name(1)))
                ; // call section window tasks
            else if (strcmp(command_target, dirs_get_section_name(2)))
                ; // call section window notes
            else if (strcmp(command_target, dirs_get_section_name(3)))
                ; // call section window projects
            else if (strcmp(command_target, dirs_get_section_name(4)))
                ; // call section window diary
        }
    else if (current_command==cmd_show) {
        open_dir(command_target);
        print_dirs_space();
        show_dirs();
    }
    free(command_target);
    command_target_index=0;
    command_target = calloc(30, sizeof(char));
    current_command=-1;
    num_spaces=0;
    last_spaceindex=0;
    clear_command_line();
    wrefresh(self);
    free_last_input_string();
    is_input_string_allocated=false;
    input_command();
}

char* mvwget_input_string_with_mouse(WINDOW *win, const int irow, const int icol, MEVENT *mouse_event, bool *pressed_mouse, bool process_dirs) {
  current_input_col=icol;
  start_reporting_mouse();
  short c = mvwgetch(win, irow, current_input_col);
  max_input_string_length = dirs_max_col - command_input_col;
  is_hinted = backspaced_whenhint = false;
  if (is_input_string_allocated)
    free_last_input_string();
  input_string = calloc(max_input_string_length , sizeof(char));
  string_builder = input_string;
  while (c!='\n'&&current_input_col!=icol+max_input_string_length-20) { /* -20 is for check_string like [d] */
    if ((c==KEY_BACKSPACE||c==127))
    {
      if (!is_hinted && !backspaced_whenhint) {
        if (current_input_col==icol)
        {
          c = wgetch(win);
          continue;
        }
        string_builder--;
        if (*string_builder==' ')
            backspacedspace=true;
        backspaced=true;
        *string_builder='1';
        current_input_col--;
        mvwprintw(self, irow, current_input_col, " ");
        wrefresh(win);
        wattroff(self,COLOR_PAIR(2));
      }
      else if (backspaced_whenhint) {
        for (int i=dirs_first_col; i<current_input_col + max_input_string_length; i++) {
        mvwprintw(self, irow, i, " ");
        wmove(self, command_input_row, dirs_first_col); }
        backspaced_whenhint=false;
        wrefresh(win);
        is_hinted = false;
        free_last_input_string();
        input_string = calloc(max_input_string_length , sizeof(char));
        current_input_col=dirs_first_col;
        string_builder = input_string;
        continue;
      } else {
        for (int i=current_input_col; i<current_input_col + max_input_string_length; i++) {
        mvwprintw(self, irow, i, " ");
        wmove(self, command_input_row, current_input_col);
        }
        is_hinted=false;
        wrefresh(win);
        backspaced_whenhint=true;
      }
    }
    if (isalpha(c)||isdigit(c)||isspace(c)||isxdigit(c)||ispunct(c))
    {
      if (is_hinted)
        for (int i=current_input_col; i< current_input_col + max_input_string_length; i++) {
          mvwprintw(self, irow, i, " ");
          wmove(self, command_input_row, current_input_col);
        }
      backspaced_whenhint=false;
      mvwprintw(win, command_input_row, current_input_col, "%c", c);
      *string_builder = c;
      string_builder++;
      current_input_col++;
      is_hinted=false;
      wrefresh(win);
      backspaced=false;
    }
      else if (c == KEY_MOUSE) {
        if (getmouse(mouse_event) == OK) {
          if (((*mouse_event).bstate & BUTTON1_CLICKED)||((*mouse_event).bstate & BUTTON1_PRESSED)||((*mouse_event).bstate & BUTTON1_RELEASED)||((*mouse_event).bstate & BUTTON1_DOUBLE_CLICKED))
            *pressed_mouse = true;
          else
            *pressed_mouse = false;
        }
        if (process_dirs)
          update_dirs_with_mouse_input(mouse_event, *pressed_mouse);
    }
    check_input_string();
    wrefresh(win);
    c = mvwgetch(win, irow, current_input_col);
  }
  *(++string_builder)='\0';
  is_input_string_allocated=true;
  return input_string;
}

void start_reporting_mouse () {
  if (is_old_mouse_mask)
    old_mouse_mask = mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, &old_mouse_mask);
  else
    old_mouse_mask = mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
  printf("\033[?1003h\n");
}

void stop_reporting_mouse () {
  mousemask(0, &old_mouse_mask);
  //printf("\033[?1003l\n");
}

void check_input_string() {
    char *c;
    if (backspacedspace)
        if (num_spaces==1)
            {
                current_command=-1;
                num_spaces=0;
                last_spaceindex=0;
                backspacedspace=false;
            }

    if (((c=strchr(input_string+last_spaceindex+1, ' ')) != NULL)) {
        if (num_spaces==0)
        {
            if (strncmp(input_string, "goto", c-input_string)==0)
                current_command=cmd_goto;
            else if (strncmp(input_string, "cd", c-input_string)==0)
                current_command=cmd_cd;
            else if (strncmp(input_string, "show", c-input_string)==0)
                current_command=cmd_show;
            else if (strncmp(input_string, "write", c-input_string)==0)
                current_command=cmd_show;
            else if (strncmp(input_string, "mkdir", c-input_string)==0)
                current_command=cmd_show;
        }
        else if (num_spaces==1)
            ;//TODO recognise other spaces
        last_spaceindex = c-input_string;
        num_spaces++;
        return;
    }
    char *p;
    if (((p=strchr(input_string, '[')) != NULL)&&(*(p+2)==']')) {
        char identifier = *(p+1);
        bool can_print=false;
        char *helpstring;
        switch (identifier)
        {
            case 's':
            if (current_command==cmd_goto) {
                if (current_section==none)
                    helpstring = "Enter section";
                else
                    helpstring = "Enter subsection";
                can_print=true;
            }
            break;
            case 'f':
            if (current_command==cmd_show) {
                can_print=true;
                switch (current_section)
                {
                    case Journal:
                        helpstring = "Enter notebook";
                        break;
                    case Notes:
                        helpstring = "Enter note";
                        break;
                    case Tasks:
                        helpstring = "Enter Task";
                        break;
                    case Diary:
                        helpstring = "Enter day";
                        break;
                    case Projects:
                        helpstring = "Enter project";
                        break;
                    default:
                        can_print=false;
                }
            }
            break;
        }
        if (can_print) {
        is_hinted=true;
        int tempcol = current_input_col;
        char *backup_string = calloc(40, sizeof(char));
        int toindex = p-input_string;
        strncpy(backup_string, input_string, toindex);
        free_last_input_string();
        input_string=backup_string;
        string_builder=input_string+toindex;
        wmove(self, command_input_row, tempcol);
        current_input_col-=3;
        wattron(self,COLOR_PAIR(3));
        mvwprintw(self, command_input_row, current_input_col, "%s", helpstring);
        wattroff(self,COLOR_PAIR(3));
        wrefresh(self);
        }
    } else if (num_spaces==1&&current_command!=-1) {
        if (backspaced&&command_target_index>0) {
            command_target_index-=2;
            backspaced=false;
        }
        
        command_target[command_target_index++] = *(string_builder-1);
    }
}

void free_last_input_string() {
  free(input_string);
}

void clear_command_line() {
    for (int i=command_input_col; i<max_input_string_length; i++) {
        mvwprintw(self, command_input_row, i, " ");
    }
}

void print_cmdshorts_title() {
    int temp_first_row = tabs_row;
    wattron(self, COLOR_PAIR(1));
    mvwprintw(self, temp_first_row, (cmd_shorts_max_col - cmdshorts_col)/2-(strlen(cmdshorts_text)/2), cmdshorts_text);
    wattroff(self, COLOR_PAIR(1));
    wrefresh(self);
}

void print_shortcuts_list() {
    wattron(self, COLOR_PAIR(2));
    for (int i=0; i<number_of_commands; i++) {
        mvwprintw(self, tabs_row+1+i, cmdshorts_col, commands[i].cmd_name);
        commands[i].tcmd_row = tabs_row+1+i;
        commands[i].tcmd_col_from = cmdshorts_col;
        commands[i].tcmd_col_to = cmdshorts_col+strlen(commands[i].cmd_name);
    }
    wattroff(self, COLOR_PAIR(2));
}

void print_border() {
    for (int i = 1, j = number_of_rows - 2; i < number_of_rows - 1 && j > 0; i++, j--) {
        mvwaddch(self, i, 0, '|');
        mvwaddch(self, j, number_of_cols - 1, '|');
    }
    for (int i = 1, j = number_of_cols - 2; i < number_of_cols - 1 && j > 0; i++, j--) {
        mvwaddch(self, 1, i, '-');
        mvwaddch(self, number_of_rows - 1, j, '-');
    }
    mvwaddch(self, 1, 0, '+');
    mvwaddch(self, number_of_rows - 1, 0, '+');
    mvwaddch(self, 1, number_of_cols - 1, '+');
    mvwaddch(self, number_of_rows - 1, number_of_cols - 1, '+');
    print_cmdw_title(" >>Command Window<< ");
}