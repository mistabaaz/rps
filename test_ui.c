#include <windows.h>
#include <stdio.h>

struct SMALL_RECT {
    SHORT Left;
    SHORT Top;
    SHORT Right;
    SHORT Bottom;
};


int main() {
    struct SMALL_RECT test;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(h, &csbi);

    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    printf("col: %d, rows: %d\n", columns, rows);

    // if (columns < 100 || rows < 30)

    system("mode CON: COLS=100 LINES=30");
    GetConsoleScreenBufferInfo(h, &csbi);

    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    printf("col: %d, rows: %d\n", columns, rows);
    
    return 0;
}