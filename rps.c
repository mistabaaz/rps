#include <stdio.h>
#include "darray.h"
#include "ui.h"

#ifdef _WIN32
#include <windows.h>
#endif

// using window size form ui.c
extern int size_x, size_y;
int legacy = 0; // for themed printing

int main(int argv, char **argc) {
    // just a start point
    long int temp_x = 0, temp_y = 0;

    if (argv == 1) {
    } else if (argv == 2 && strcmp(argc[1], "--legacy") == 0) {
        legacy = 1;
    } else if (argv == 3) {
        temp_x = strtol(argc[1], NULL, 10);
        temp_y = strtol(argc[2], NULL, 10);
    } else if (argv == 4 && strcmp(argc[3], "--legacy") == 0 ) {
        temp_x = strtol(argc[1], NULL, 10);
        temp_y = strtol(argc[2], NULL, 10);
        legacy = 1;  // disables themed printing
    } else {
        printf("usage : %s [size_x size_y] [--legacy]\n", argc[0]);
        printf("example : %s 80 27 --legacy\n", argc[0]);
        printf("      or  %s --legacy\n", argc[0]);
        return -1;
    }
    
    if (temp_x != 0 && temp_y != 0) {
        size_x = temp_x;
        size_y = temp_y;
    }

    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return -1;
    }
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return -1;
    }
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
    if (!SetConsoleMode(hOut, dwMode)) {
        return -1;
    }

    // created an alternate screen buffer
    HANDLE hAlt = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    if (hAlt == INVALID_HANDLE_VALUE) {
        printf("error when creating alternate input handle");
        return -1;
    }
    SetConsoleActiveScreenBuffer(hAlt);
    SetStdHandle(STD_OUTPUT_HANDLE, hAlt);
    // SetStdHandle(STD_INPUT_HANDLE, hAlt);
    #endif

    int val = change_terminal_settings(size_x, size_y);
    if (val == -1) {
        printf("can't enable vt-100 escap sequence!\n");
        return -1;
    }

    run(legacy);

    #ifdef _WIN32
    CloseHandle(hAlt);
    SetConsoleActiveScreenBuffer(hOut);
    SetStdHandle(STD_OUTPUT_HANDLE, hOut);
    #endif

    return 0;
}
