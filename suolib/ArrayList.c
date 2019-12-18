#include "suolib/ArrayList.h"
#include <stdbool.h>

bool remove_at(int array[], int *len, int index) {
    if (index>*len||index<0)
        return false;
    for (int i=index; i<*len-1; i++)
    {
        array[i] = array[i+1];
    }
    *len-=1;
    return true;
}

bool remove_range(int array[], int startindex, int endindex) {
    if (startindex>endindex||startindex<0)
        return false;
    
}