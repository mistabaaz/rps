#include <stdio.h>
#include "darray.h"
#include "ui.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    // just a start point
    int size_x = 80, size_y = 27;

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

    int val = enable_terminal_settings(size_x, size_y);
    if (val == -1) {
        printf("can't enable vt-100 escap sequence!\n");
        return -1;
    }

    run();

    #ifdef _WIN32
    CloseHandle(hAlt);
    SetConsoleActiveScreenBuffer(hOut);
    SetStdHandle(STD_OUTPUT_HANDLE, hOut);
    #endif

    return 0;
}
