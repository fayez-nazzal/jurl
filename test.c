#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"
#include "list.h"

int main() {
    ArrayList AL = *arraylist_new(20);
    int val=12;
    arraylist_insert(&AL, 0, &val);
    printf("%i\n", *(int *)AL.data[0]);

    ListEntry *LL = NULL;
    int values[] = {2,3,4,1,6,34,75,-1};
    for (int i=0; values[i]!=-1; i++) {
    list_append(&LL, &values[i]);
    }

    ListIterator LI;
    list_iterate(&LL, &LI);
    while(list_iter_has_more(&LI)) {
    printf("%i\n", *(int *)list_iter_next(&LI));
    }
}