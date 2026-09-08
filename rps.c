#include <stdio.h>
#include "darray.h"
#include "ui.h"

int main() {
    // just a start point
    int size_x = 80, size_y = 27;
    int val = enable_terminal_settings(size_x, size_y);
    if (val == -1) {
        printf("can't enable vt-100 escap sequence!\n");
        return -1;
    }

    run();
    return 0;
}
