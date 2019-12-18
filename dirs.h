#ifndef DIRS_H
#define DIRS_H

typedef struct _dir_text dir_text;
struct _dir_text {
    char *dir_name;
    int dir_row;
    int dir_col_from;
    int dir_col_to;
} *directories;
int number_of_dirs;

void get_directories(char* dirname);

void delete_source_dirs();

#endif
