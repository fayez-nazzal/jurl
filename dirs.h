#ifndef DIRS_H
#define DIRS_H
#include <stdlib.h>
typedef struct _dir_text dir_text;
struct _dir_text {
    char *name;
    int row;
    int col_from;
    int col_to;
} *directories;
int number_of_dirs;
enum section {none=-1, Journal=0, Tasks=1, Notes=2, Projects=3, Diary=4};
int current_section;
bool open_dir(char* dirname);
char* dirs_get_section_name(const int num);
#endif
