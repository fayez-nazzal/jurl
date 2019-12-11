#ifndef INPUT_H
#define INPUT_H
#include <ncurses.h>
mmask_t old_mouse_mask;
int mvwget_input_with_mouse(WINDOW *win, int row, int col, MEVENT *mouse_event);
int mvwget_input(WINDOW *win, int row, int col);
char* mvwget_input_string_with_mouse(WINDOW *win,const int irow,const int icol, MEVENT *mouse_event, bool *pressed_mouse, bool process_dirs);
char* mvwget_input_string(WINDOW *win, const int irow, const int icol);
void stop_reporting_mouse ();
void start_reporting_mouse ();
#endif
