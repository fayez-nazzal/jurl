#ifndef DIRS_H
#define DIRS_H

struct directory_text {
    char *dir_name;
    int dir_row;
    int dir_col_from;
    int dir_col_to;
};
struct directory_text *directories;
int number_of_dirs;

void get_directories();

void delete_source_dirs();

#endif
