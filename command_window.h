#ifndef COMMAND_WINDOW_H
#define COMMAND_WINDOW_H

#include <ncurses.h>
#include "dirs.h"
#include "globals.h"
int command_row;
void init_command_window();
void command_window_show_directories();
void update_dir_window();
void update_dirs_mouse_input(MEVENT *mouse_event, bool pressed_mouse);
#endif
