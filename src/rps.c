#include <stdio.h>
#include <stdlib.h> // for strtol()
#include <string.h> // for strcmp()
#include "darray.h"
#include "ui.h"

#ifdef _WIN32
#include <windows.h>
#endif

int init_args(int, char**);
void usage(char *);

// using window size form ui.c
extern int size_x, size_y;
int legacy = 0; // for themed printing
int snapshots = 0;

int main(int argv, char **argc) {
    // just a start point
    if (init_args(argv, argc) == -1) {
        return -1;
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

    // switching to alternate buffer using escape code
    #ifndef _WIN32
    printf("\x1b[?1049h");
    #endif

    int val = change_terminal_settings(size_x, size_y);
    if (val == -1) {
        printf("can't enable vt-100 escap sequence!\n");
        return -1;
    }

    run(legacy, snapshots);

    #ifdef _WIN32
    CloseHandle(hAlt);
    SetConsoleActiveScreenBuffer(hOut);
    SetStdHandle(STD_OUTPUT_HANDLE, hOut);
    #endif

    // restoring alternate buffer
    #ifndef _WIN32
    printf("\x1b[?1049l");
    #endif

    return 0;
}


int init_args(int argv, char **argc) {
    long int temp_x = 0, temp_y = 0;

    int init = 0;
    if (argv == 1) {
    } else if (argv == 2) {
        if ( strcmp(argc[1], "--legacy") == 0) {
            legacy = 1;  // disables themed printing
        } else if ( strcmp(argc[1], "--snapshots") == 0) {
            snapshots = 1; // enable screenshot
        } else {
            usage(argc[0]);
            return -1;
        }
    } else if (argv == 3) {
        temp_x = strtol(argc[1], NULL, 10);
        temp_y = strtol(argc[2], NULL, 10);
        if ( strcmp(argc[1], "--legacy") == 0 && strcmp(argc[2], "--snapshots") == 0) {
            legacy = 1;           
            snapshots = 1;
        } else if (temp_x == 0 && temp_y == 0) {
            usage(argc[0]);
            return -1;
        }
    } else if (argv == 4) {
        temp_x = strtol(argc[1], NULL, 10);
        temp_y = strtol(argc[2], NULL, 10);
        if (strcmp(argc[3], "--legacy") == 0) {
            legacy = 1;
        } else if ( strcmp(argc[3], "--snapshots") == 0 ) {
            snapshots = 1;
        } else {
            usage(argc[0]);
            return -1;
        }
    } else if (argv == 5) {
        temp_x = strtol(argc[1], NULL, 10);
        temp_y = strtol(argc[2], NULL, 10);
        if (strcmp(argc[3], "--legacy") == 0 && strcmp(argc[4], "--snapshots") == 0) {
            legacy = 1;
            snapshots = 1;
        } else {
            usage(argc[0]);
            return -1;
        }
    } else {
        usage(argc[0]);
        return -1;
    }
    
    if (temp_x != 0 && temp_y != 0) {
        size_x = temp_x;
        size_y = temp_y;
    }

    return 0;
}

void usage(char *filename) {
    printf("usage : %s [size_x size_y] [--legacy] [--snapshots]\n", filename);
    printf("example : %s 80 27 --legacy --snapshots\n", filename);
    printf("      or  %s --legacy\n", filename);
}
