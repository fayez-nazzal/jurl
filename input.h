#ifndef INPUT_H
#define INPUT_H
#include <ncurses.h>
mmask_t old_mouse_mask;
int current_input_col;
void free_last_input_string();
char* mvwget_input_string_with_mouse(WINDOW *win, const int irow, const int icol, MEVENT *mouse_event, bool *pressed_mouse, bool process_dirs);
void stop_reporting_mouse ();
void start_reporting_mouse ();
void *mouse_thread(void *vargp);
#endif
