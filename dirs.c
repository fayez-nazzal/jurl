#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include "dirs.h"

bool is_directories_allocated = false;

void get_directories(char* dirname) {
    if (is_directories_allocated)
        free(directories);
    directories = calloc(100, sizeof(dir_text));
    is_directories_allocated = true;
    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);
    number_of_dirs = 0;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char *dir_name = calloc(strlen(dir->d_name), sizeof(char));
            strcpy(dir_name, dir->d_name);
            directories[number_of_dirs].dir_name = dir_name;
            number_of_dirs++;
        }
        closedir(d);
    }
    delete_source_dirs();
}

void delete_source_dirs() {
    /* the element number 1 should be the executable name */
    number_of_dirs--;
    for (int j = 1; j < number_of_dirs; j++) {
        directories[j] = directories[j + 1];
    }
    for (int i = 0; i < number_of_dirs; i++) {
        char *dir_name = directories[i].dir_name;
        int length = strlen(dir_name);
        if (dir_name[0]=='.' ||
            (dir_name[length - 2] == '.' && (dir_name[length - 1] == 'h' || dir_name[length - 1] == 'c'))) {
            if (strcmp(".",dir_name)!=0) {
            (number_of_dirs)--;
            for (int j = i; j < number_of_dirs; j++) {
                directories[j] = directories[j + 1];
            }
            i--;
            }
        }
    }
}
