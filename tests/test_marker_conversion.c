#include "markers.h"
#include <stdio.h>
int main () {
    int i;
    /*for (i=0;i<256;i++) {
        printf("%2x: %s\n", i, get_marker_name(i));
    }*/
    for (i=0;i<256;i++) {
        char *name = get_marker_name(i);
        int number = get_marker_number(name);
        if (number!=i) {
            printf("marker %5d, %3x : %10s : %5d, %3x\n", i, i, name, number, number);
        }
    }
    return 0;
}