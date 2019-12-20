#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include "dirs.h"

static bool isdirs_allocated = false;
static const int sectionsnum = 5;
static char *section_names[] = {"Journal", "Tasks", "Notes", "Projects", "Diary"};

char* dirs_get_section_name(const int num) {
    if (num>4||num<0)
        {
            endwin();
            fprintf(stderr, "wrong section num");
        }
    return section_names[num];
}

bool open_dir(char* dirname) {
    static DIR *d;
    static struct dirent *dir; 
    number_of_dirs=2; // for "." and "show window"
    static char* extension;
    if (isdirs_allocated)
        free(directories);
    if (strcmp(dirname, ".")==0||strcmp(dirname, "..")==0) {
        number_of_dirs=5;
        directories = calloc(number_of_dirs, sizeof(dir_text));
        for(int i=0; i<sectionsnum; i++) {
            directories[i].name = section_names[i];
        }
        return true;
    }
    else if ((extension = strstr(dirname, ".ju"))!=NULL)
    {
        extension+=3;
        switch(*extension) {
            case 'j':
            break;
            case 't':
            break;
            case 'n':
                exit(1);
            break;
            case 'p':
            break;
            case 'd':
            break;
            default:
                endwin();
                fprintf(stderr, "Error, wrong file extension\n");
        }
        number_of_dirs=0;
        return false;
    }
    /* count number of directories */


    d = opendir(dirname);
    if (!d)
        return false;
    
    for (int i=0; i<sectionsnum; i++) {
        if (strcmp(dirname, section_names[i])==0) {
            current_section=i;
            break;
        }
    }
    while ((dir = readdir(d)) != NULL) {
        if (strstr(dir->d_name,  ".ju")!=NULL)
            number_of_dirs++;
    }

    rewinddir(d); /* reset to the beginning */

    directories = malloc(number_of_dirs * sizeof(dir_text));
    isdirs_allocated = true;

    directories[0].name = "show window";
    directories[1].name = "..";

    int index=2;
    while ((dir = readdir(d)) != NULL) {
        if (strstr(dir->d_name,  ".ju")!=NULL) {
        char *dir_name = calloc(strlen(dir->d_name), sizeof(char));
        strcpy(dir_name, dir->d_name);
        directories[index].name = dir_name;
        index++;
        }
    }
    closedir(d);
    return true;
}