/*
Author : mistabaaz
Date : 22 Oct 2024
Description : A simple game Rock, Paper and Secior but with a unique style.
*/

/*
and today i open my laptop just to continue this project
and i saw 22 oct 2025 ..... ahhh......
one year gone but stil can't complete it due to inconsistency
*/

#include <stdio.h>  // printf(), scanf()
#include <stdlib.h> // srand(), system()
#include <string.h> // strcpy(), strlen()
#include <time.h>   // time()

#ifdef _WIN32
#include <windows.h>  // for many internal apis
// #include <wchar.h>
#include <conio.h> // for _getch()
#endif

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#ifndef DISABLE_NEWLINE_AUTO_RETURN
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008
#endif

#include "darray.h"  // for dynamic string array
#include "art.h"     // for ascii art
#include "ui.h"      // for design

// result matrix to
int result[3][3] = {
    {2, 0, 1}, // 0 means user lose
    {1, 2, 0}, // 1 means user win
    {0, 1, 2}  // 2 means draw
};

int score[2] = {0, 0};

int run() {
    // Crispy part is here :)
    
    // struct ascii_arts pointer from left hand to right
    char ***arts = gen_art();

    // allocating memory for box structure
    xy_plane *box = gen_xyplane(80, 27);

    static char *logo[] = {
        "██▀███      ██▓███       ██████  ",
        "▓██ ▒ ██▒   ▓██░  ██▒   ▒██    ▒ ",
        "▓██ ░▄█ ▒   ▓██░ ██▓▒   ░ ▓██▄   ",
        "▒██▀▀█▄     ▒██▄█▓▒ ▒     ▒   ██▒",
        "░██▓ ▒██▒   ▒██▒ ░  ░   ▒██████▒▒",
        "░ ▒▓ ░▒▓░   ▒▓▒░ ░  ░   ▒ ▒▓▒ ▒ ░",
        "  ░▒ ░ ▒░   ░▒ ░        ░ ░▒  ░ ░",
        "  ░░   ░    ░░          ░  ░  ░  ",
        "   ░                          ░  ",
        "                                 "
    };

    int logo_len = 8;
    int str_size = strlen(logo[logo_len + 1]); // normal space length without unicode
    // Q: does strlen() calculates \0 in length ? A: no
    int center = ( *(box->size_x + 0) - str_size) / 2;  // center coordinates(x-axis) of the box

    int nbox_bloc[2][2] = { {*(box->len_x + 0) - 23, 2}, {*(box->len_x + 0) - 2, 5} };  // embeded box coordinates
    // int nbox_bloc[2][2] = { {100-23, 2}, {99, 5}};
    int name_len = nbox_bloc[1][0] - nbox_bloc[0][0] - 1 - 3;  // here that is 20 - 3
    // -3 for score

    if (name_len < strlen("Computer")) {
        printf("name_len is too small !!");
        return 1;
    }

    char *Player1 = malloc(name_len);
    char *Player2 = malloc(name_len);
    strcpy(Player2, "Computer");

    if ((Player1 == NULL) || (Player2 == NULL)) {
        printf("Cannot assign memory to player names!\n");
        return 1;
    }

    printf("\nEnter Player name: ");
    fgets(Player1, name_len, stdin); // Q: does fgets put \0 at name_len or name_len + 1?
    Player1[strlen(Player1)] = '\0';
    printf("\n");

    if (*Player1 == '\0') {
        *Player1 = ' ';
        printf("you skipped the name part;\n\n");
        fflush(stdout);
        strcpy(Player1, "Player1");
    }

    create_box(box);

    embed_box(box, nbox_bloc, "name_box");
    int nbox_loc[2][2];
    darray_get_loc_by_name(box->elements, "name_box", nbox_bloc, nbox_loc);
    write_line(box, nbox_loc[0][0], nbox_loc[1][0], nbox_loc[0][1], Player1);
    write_line(box, nbox_loc[0][0], nbox_loc[1][0], nbox_loc[1][1], Player2);

    // adding logo
    ucode *u1 = gen_ucode();
    int b1[2][2] = { {center - 10, 2} , {center - 10 + strlen(logo[logo_len]), logo_len + 1} };
    darray_append(box->elements, "logo", b1, b1, u1);
    
    // unicode writer needs debugging [done]
    for (int j = 0; j < logo_len; j++) {
        unicode_writer(box, j + 2, center - 10, logo[j], str_size, "logo");
        // color_writer(box, center - 10, center - 10 + strlen(logo[j]), j + 2, 31);
    }    

    // color_unicode(box, "logo", 31);
    color_by_name(box, "logo", 031);
    color_by_name(box, "name_box", 032);
    // decolor(box, nbox_bloc[0][0] + 5, nbox_bloc[0][0] + name_len + 1 + 5, nbox_bloc[0][1] + 1);
    decolor_by_name(box, "name_box");
    decolor_by_name(box, "logo");

    // element choice
    char *ask = "Enter choice: ";
    int padding = 4;
    int choice_loc[2][2] = { { padding, logo_len + 5}, {strlen(ask) + padding + 1, logo_len + 5} };
    write_line(box, padding, strlen(ask) + padding + 1, logo_len + 5, ask);
    darray_append(box->elements, "choice", choice_loc, choice_loc, NULL);


    /** a simple note*/
    char *note = "(q for quit)";
    padding = 4;
    int note_loc[2][2] = { {*(box->len_x) - strlen(note) - padding, logo_len + 5}, { *(box->len_x) - padding, logo_len + 5}};
    write_line(box, note_loc[0][0], note_loc[1][0], note_loc[0][1], note);
    darray_append(box->elements, "note", note_loc, note_loc, NULL);
    
    color_writer(box, note_loc[0][0] + 7, note_loc[1][0] - 2, note_loc[0][1], 41);
    /** end of note */

    print_box(box);
    // fflush(stdout);
    
    int midx = *(box->len_x + 0) / 2;
    int padd = midx - 25;
    int left_loc[2][2] = { { padd, logo_len + 8}, {art_width + padd, logo_len + 7 + art_height} };
    darray_append(box->elements, "left_art", left_loc, left_loc, NULL);

    int right_loc[2][2] = { {*(box->len_x) - art_width - padd, logo_len + 8}, { *(box->len_x) - padd, logo_len + 7 + art_height}};
    darray_append(box->elements, "right_art", right_loc, right_loc, NULL);


    char *cc = malloc( name_len + 20 );
    char *chosed = malloc(9); // for computer choice
    char *win = malloc(name_len + 5); // for winner name
    char *scr = malloc(3); // for scorecard
    while (1) {
        // trying to move cursor to ask position
        int up_down = 1 * (box->len_y - choice_loc[0][1] + 1);
        int right_left = 1 * ( padding + strlen(ask) );
        // printf("updown: %d, right_left: %d\n", up_down, right_left);
        move_cursor(up_down, right_left);
        
        // ask for input char 1 2 3 r p s q
        #ifdef _WIN32
        int c = _getch(); // instead disable canonical mode
        #endif
        
        int cleaned = darray_get_pos(box->elements, "win");
        if (cleaned != -1) {
            element_fill(box, "left_art", ' ');
            element_fill(box, "right_art", ' ');
            element_fill(box, "chosed", ' ');
            element_fill(box, "win", ' ');
        }

        int user;
        if ( c == 'r' || c == '1') {
            // char chosen[] = "you choosed rps!";
            // write_line(box, padding, strlen(chosen), logo_len + 5, chosen);
            user = 0;
        } else if ( c == 'p' || c == '2') {
            user = 1;
        } else if (c == 's' || c == '3') {
            user = 2;
        } else if (c == 'q' || c == '0') {
            // exit
            free(Player1);
            free(Player2);
            free_arts(arts);

            free(cc); free(chosed); free(win);

            int r = ud_json_file(box->elements);
            free_box(box);
            return 0;
        }

        int comp = comp_choice();
        if (result[user][comp] == 0) {
            score[1]++;
        } else if ( result[user][comp] == 1 ) {
            score[0]++;
        }

        darray_get_loc_by_name(box->elements, "name_box", nbox_bloc, nbox_loc);
        int inc1 = calculate_increment(box, nbox_loc[0][1], nbox_loc[1][0]);
        int inc2 = calculate_increment(box, nbox_loc[1][1], nbox_loc[1][0]);

        snprintf(scr, 2, "%d", score[0]);
        write_line(box, nbox_loc[1][0] + inc1 - 2, nbox_loc[1][0] + inc1, nbox_loc[0][1], scr);

        snprintf(scr, 2, "%d", score[1]);
        write_line(box, nbox_loc[1][0] + inc2 - 2, nbox_loc[1][0] + inc2, nbox_loc[1][1], scr);

        struct art_couple a;
        a = get_art(user, comp, arts);
        
        for (int i = 0; i < art_height; i++) {
            write_line(box, left_loc[0][0], left_loc[1][0], left_loc[0][1] + i, a.left[i] );
        }

        for (int i = 0; i < art_height; i++) {
            write_line(box, right_loc[0][0], right_loc[1][0], right_loc[0][1] + i, a.right[i] );
        }

        if (comp == 0) {
            strcpy(chosed, "rock");
        } else if (comp == 1) {
            strcpy(chosed, "paper");
        } else {
            strcpy(chosed, "scissors");
        }
        sprintf(cc, "%s choosed %s", Player2, chosed);
        
        
        // writing what computer chosed
        int chosed_loc[2][2] = { {midx - 1 - ( strlen(cc) / 2 ), logo_len + 8 + art_height + 2}, {midx + ( strlen(cc) / 2 ), logo_len + 8 + art_height + 2} };
        write_line(box, chosed_loc[0][0], chosed_loc[1][0], chosed_loc[0][1], cc);
        int p = darray_get_pos(box->elements, "chosed");
        if ( p == -1 )  {
            darray_append(box->elements, "chosed", chosed_loc, chosed_loc, NULL);
        } else {
            darray_update_loc_by_name(box->elements, "chosed", chosed_loc, chosed_loc);
        }

        int res = result[user][comp];
        if (res == 0) {
            sprintf(win, "%s WIN", Player2);    
        } else if (res == 1) {
            sprintf(win, "%s WIN", Player1);
        } else {
            strcpy(win, "TIE");
        }

        int win_loc[2][2] = { {midx - 1 - ( strlen(win) / 2 ), logo_len + 8 + art_height + 3}, {midx + ( strlen(win) / 2 ), logo_len + 8 + art_height + 3} };
        write_line(box, win_loc[0][0], win_loc[1][0], win_loc[0][1], win);
        p = darray_get_pos(box->elements, "win");
        if ( p == -1 )  {
            darray_append(box->elements, "win", win_loc, win_loc, NULL);
        } else {
            darray_update_loc_by_name(box->elements, "win", win_loc, win_loc);
        }
        

        print_box(box);

        // fix clearing what written [done]
        // make score update [done]
    }

    // printf("hello%d\n", r);

    return 0;
}

int user_choice() {
    unsigned int choice;

    printf("\n\t1 Rock\n\t2 Paper\n\t3 Scissor\n");
    printf("\nEnter 4 to quit:");
    scanf("%u", &choice);
    getchar(); // to overcome \n issue
    printf("\n");

    if (choice > 4) {
        printf("\nPlease Choose Correct Option!\n");
        choice = user_choice();
    }

    return choice - 1;
}

int comp_choice() {
    // generate random number from 0, 1 or 2
    time_t seed = time(NULL);
    srand(seed); // for creating randomness using time

    int choice = rand() % 3;  // for generating number < 3
}

int normal_style() {
    // normal view for rock paper scissor game
    int user, comp;
    char *choices[3] = {"Rock", "Paper", "Scissor"};
    // for displaying computer choice

    while (1) {
        printf("\n\nRock Paper Scissor\n");
        user = user_choice();
        comp = comp_choice();
        printf("Computer choosed %s\n", choices[comp]);

        if (user == 3) { // 3 means exit
            printf("Okay leaving....\n");
            break;
        }

        int winner = result[user][comp]; // gives winner index

        if (winner == 0) {
            printf("\nComputer is the Winner :( \n");
        } else if (winner == 1) {
            printf("\nYou are the Winner :)\n");
        } else if (winner == 2) {
            printf("\nMatch Drawn :| \n");
        } else {
            printf("\nEven computer dosen't know who wins ^-^\n");
        }
    }

    return 0;
}

int enable_terminal_settings(int size_x, int size_y) {
    // enable unicode & escape sequence in windows
    
    #ifdef _WIN32
    // Requires windows ver 1511 or above
    // for enabling VT-100 in windows or system(" ");
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

    // Output mode : CP_UTF8 or system(chcp 65001);
    SetConsoleOutputCP(65001);
    SetConsoleTitle("mrps");

    HWND handle = FindWindow(NULL, "mrps");
    if (handle == NULL) {
        return -1;
    }

    // trying to get pixel length of char
    HDC hDC = GetDC(handle);
    if (hDC == NULL) {
        return -1;
    }
    RECT r = { 0, 0, 0, 0};
    char str[] = "A";
    DrawText(hDC, str, strlen(str), &r, DT_CALCRECT | DT_NOPREFIX | DT_SINGLELINE);
    // printf("%d %d %d %d\n", r.left, r.right, r.top, r.bottom);
    int p_width = r.right - r.left;  // 8 is general char width
    int p_height = r.bottom - r.top; // 16 general char height in px

    int pixel_x = (size_x + 0) * p_width;
    int pixel_y = (size_y + 5) * p_height;

    int padding_x = 10 * p_width;
    int padding_y = 3 * p_height;
    SetWindowPos(handle, NULL, padding_x, padding_y, pixel_x, pixel_y, SWP_NOZORDER);

    // get buffer size and re apply it to disable scroll bar
    COORD c;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    c.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    c.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    // BOOL ok = SetConsoleScreenBufferSize(hOut, c);
    // ok = ShowScrollBar(handle, SB_VERT, 0);
    // if (!ok) printf("cant disable scroll bar!\n");

    // another way system(change_size);
    // char change_size[50];
    // sprintf(change_size ,"mode CON: COLS=%d LINES=%d", size_x + 2, size_y + 4);
    
    // change cursor style
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    // printf("Current %%filled %d, visibility %d\n", cci.dwSize, cci.bVisible);

    cci.dwSize = 100;
    cci.bVisible = 1;
    SetConsoleCursorInfo(hOut, &cci);

    // initial default set by windows is 500ms
    int blink_rate = GetCaretBlinkTime();
    // printf("current cursor blink rate: %d\n", blink_rate);
    // SetCaretBlinkTime(500);

    // fixed cursor blinking
    ShowWindow(handle, SW_MINIMIZE);
    ShowWindow(handle, SW_NORMAL);

    // another way to change cursor style
    // printf("\x1b[1 q");

    ReleaseDC(NULL, hDC);
    #endif
    return 0;
}

xy_plane* gen_xyplane(int x, int y) {
    // return pointer to allocated memory of box
    char **rows;
    int *size_x = malloc(sizeof (int) * (y + 2) );
    int *len_x = malloc(sizeof (int) * (y + 2) );

    if (size_x == NULL || len_x == NULL) {
        printf("memory not allocated for size_x & len_x\n");
        return NULL;
    }
    for (int i = 0; i < y + 2; i++) {
        *(size_x + i) = x + 4; // +4 for ||\n\0
        *(len_x + i) = x;
    }

    darray *elements = gen_darray();
    xy_plane *box = malloc(sizeof (xy_plane));
    box->rows = rows;
    box->size_x = size_x;
    box->size_y = y + 2;  // for first and last line
    box->len_x = len_x;
    box->len_y = y;
    box->elements = elements;

    return box;
}

void row_creator(char *temp_row, int size_x, char corner1, char space, char corner2, int nextline, int null_tag) {
    // create a row with customized characters

    *temp_row = corner1;

    for (int i = 1; i < size_x + 1; i++) {
        *(temp_row + i) = space;
    }

    *(temp_row + size_x + 1) = corner2;

    if (nextline == 1) {
        *(temp_row + size_x + 2) = '\n';
    }

    if (null_tag == 1) {
        *(temp_row + size_x + 3) = '\0';
    }
}

void create_box(xy_plane *box) {
    // create empty 2d box with borders
    char **rows = malloc( (box->size_y) * sizeof(char *) );
    char *temp_row;

    temp_row = malloc( *(box->size_x + 0) );
    if (temp_row == NULL) {
        printf("Cannot assign memory to first line!\n");
    }

    row_creator(temp_row, *(box->len_x + 0), '+', '-', '+', 1, 1);
    *rows = temp_row; // stored first line

    for (int i = 1; i < box->size_y - 1; i++) {
        temp_row = malloc(*(box->size_x + i) ); // +4 for ||\n\0
        if (temp_row == NULL) {
            printf("Cannot assign memory to %dth line!\n", i);
        }

        row_creator(temp_row, *(box->len_x + i), '|', ' ', '|', 1, 1);
        *(rows + i) = temp_row;
    }

    temp_row = malloc(*(box->size_x + box->size_y - 1));
    if (temp_row == NULL) {
        printf("Cannot assign memory to last line!\n");
    }

    row_creator(temp_row, *(box->len_x + box->size_y - 1), '+', '-', '+', 0, 1);
    *(rows + box->size_y - 1) = temp_row; // stored last line

    box->rows = rows;
}

void free_box(xy_plane *box) {
    // free the pointer memory of box
    char *temp_row;

    for (int i = 0; i < box->size_y; i++) {
        temp_row = *(box->rows + i);
        // printf("row %d temp %d", i, temp_row);
        free(temp_row);
    }

    free(box->rows);
    free(box->size_x);
    free(box->len_x);
    darray_free(box->elements);
    free(box);
}

void print_box(xy_plane *box) {
    // print all rows from box

    printf("\n");
    for (int i = 0; i < box->size_y; i++) {
        printf("%s", *(box->rows + i));
    }

    // printf("\n");
}

void embed_box(xy_plane *box, int loc[2][2], char *element) {
    // makes another box inside the outer box :)
    int pos_x1 = loc[0][0], pos_x2 = loc[1][0];
    int pos_y1 = loc[0][1], pos_y2 = loc[1][1];

    char *temp_row;

    if (pos_y2 - pos_y1 < 1) {
        printf("\nbox which you want to embed have size 0x0\n");
        return;
    }
    if (pos_y1 > box->len_y - 1) {
        printf("coordinates lies outside the box\n");
        return;
    }
    if ((pos_y1 < 1) || (pos_y2 > box->len_y)) {
        printf("\ncoordinates lies outside the box!\n");
        printf("when embeding the box\n");
        return;
    }
    if ((pos_x1 < 1) || (pos_x2 > *(box->len_x + pos_y1))) {
        printf("\ncoordinates lies outside the box!\n");
        printf("when embeding the box\n");
        return;
    }

    // first row
    temp_row = *(box->rows + pos_y1);
    temp_row = temp_row + pos_x1;
    row_creator(temp_row, (pos_x2 - pos_x1 - 1), '-', '-', '-', 0, 0);

    for (int i = pos_y1 + 1; i < pos_y2; i++) {
        temp_row = *(box->rows + i);
        // *temp_row = '-';
        temp_row = temp_row + pos_x1; // for offset in x

        row_creator(temp_row, (pos_x2 - pos_x1 - 1), '|', ' ', '|', 0, 0);
    }

    // last row
    
    temp_row = *(box->rows + pos_y2);
    temp_row = temp_row + pos_x1;
    row_creator(temp_row, (pos_x2 - pos_x1 - 1), '-', '-', '-', 0, 0);

    // int bloc[2][2] = { {pos_x1, pos_y1}, {pos_x2 , pos_y2} }; // actual pos
    int tloc[2][2] = { {pos_x1 + 1, pos_y1 + 1}, {pos_x2 - 1, pos_y2 - 1} }; // loc inside box

    // making it unicode for correct implementing increment
    // see it correctness
    // ucode *uc = gen_ucode();
    // for (int i = pos_y1; i < pos_y2 + 1; i++) {
    //     ucode_append(uc, tloc, loc);
    // }
    darray_append(box->elements, element, loc, tloc, NULL);  // added as element
}

void write_line(xy_plane *box, int pos_x1, int pos_x2, int pos_y, char *string) {
    // write contents into box rows
    // write the line till \n or given size
    char *temp_row;

    // check if there is unicode before y
    int inc = calculate_increment(box, pos_y, pos_x1);

    if ((pos_y < 1) || (pos_y > box->len_y)) {
        printf("\ncoordinates lies outside the box!\n");
        printf("when writing this line \"%s\"\n", string);
        return;
    }
    if (pos_y > box->len_y - 1) {
        printf("coordinates lies outside the box\n");
        return;
    }
    if ((pos_x1 < 1) || (pos_x2 > *(box->len_x + pos_y) + inc)) {
        printf("\ncoordinates lies outside the box!\n");
        printf("when writing this line \"%s\"\n", string);
        return;
    }
    if ( strlen(string) > pos_x2 - pos_x1) {
        printf("\nstring length is greater than given space!\n");
        printf("writing uncomplete line\n");
    }

    temp_row = *(box->rows + pos_y);
    for (int i = 0; i <= pos_x2 - pos_x1; i++) {
        if ((string[i] == '\0') || (string[i] == '\n')) {
            break;
        }
        *(temp_row + pos_x1 + i) = string[i];
    }
}

void clear_lines(int lines, int backward, int all, int one) {
    // for clearing one , all lines

    if (all == 1) {
        printf("\x1b[1J\n"); // full screen clear
        return;
    }
    if (one == 1) {
        printf("\x1b[2K\n"); // current line clear
        return;
    }

    if (backward == 1) {
        for (int i = 0; i < lines; i++) {
            printf("\x1bM"); // move up one by one
        }
    }

    for (int i = 0; i < lines; i++) {
        printf("\x1b[2K\n"); // clear line one by one
    }

    for (int i = 0; i < lines; i++) {
        printf("\x1bM"); // move cursor up after clearing
    }
}

void move_cursor(int up_down, int right_left) {
    // move cursor left right up down

    char x = 'F'; // for moving up
    char y = 'C'; // for moving cursor right
    if (right_left < 0) {
        y = 'D'; // for moving cursor to left control key
        right_left = (-1) * right_left;
    }
    if (up_down < 0) {
        x = 'E'; // for moving below (down line)
        up_down = (-1) * up_down;
    }

    if (up_down != 0) {
        printf("\x1b[%d%c", up_down, x);
        // printf("%d%c", up_down, x);
    }
    if (right_left != 0) {
        printf("\x1b[%d%c", right_left, y);
        // printf("%d%c", right_left, y);
    }
}

void inc_elements(xy_plane *box, int pos_y, int pos_x1, int increment) {
    // if any new element takes extra space this helps
    int bloc[2][2];
    int loc[2][2];
    int *uv = box->elements->uval;
    for (int i = 0; i < box->elements->lpos; i++) {
        // if unicode 
        int is_ucode = *(uv + i);
        if (is_ucode == 1) {
            // dont hvae uvalues len using j index
            ucode *temp = *(box->elements->u + i);
            for (int k = 0; k < temp->len; k++) {
                uget_bxy(temp, k, bloc);
                // uget_xy(temp , k , loc);
                
                // check if this should be inc or not
                int condition = ( bloc[0][1] == pos_y ) && ( bloc[0][0] > pos_x1 );
                if (condition) {
                    bloc[0][0] += increment; // initial
                    bloc[1][0] += increment; // final
                }
                uset_bxy(temp, k, bloc);
            }
        }
    }
}

void x_shifter(xy_plane *box, int pos_x1, int pos_x2, int pos_y, int shift_by) {
    /* shift the characters in x axis in 2d box 
    + means forword shift - means backward shift */

    if ( (shift_by > 0) && (pos_x2 + shift_by > *(box->size_x + pos_y )) ) {
        printf("cannot shift more than array size!");
        return;
    }
    if ( (shift_by < 0) && (pos_x1 + shift_by < 1) ) {
        printf("cannot shift backward from where the array begins!");
        return;
    }

    if (shift_by > 0) {
        // means forword shifting
        char *arr = *(box->rows + pos_y);

        for (int i = 0; i < shift_by; i++) {
            for (int j = pos_x2 + i + 1; j > pos_x1 + i; j--) {
                *(arr + j) = *(arr + j - 1); // copied last second element into last
            }
        }

        // updated len in box
        // *(box->size_x + pos_y) += shift_by;

    }

    if (shift_by < 0) {
        // means backward shifting
        // means data will be overwritten
        shift_by = (-1) * shift_by;
        char *arr = *(box->rows + pos_y);

        for (int i = 0; i < shift_by; i++) {
            for (int j = pos_x1 - i; j < pos_x2 - i + 1; j++) {
                *(arr + j - 1) = *(arr + j); // copied first element 
            }
        }

        // updated len in box
        // *(box->size_x + pos_y) -= shift_by;

    }

}

void unicode_writer(xy_plane *box, int pos_y, int pos_x, char *uni_string, int free_space, char *element) {
    // write single row of unicode string

    /*  this function is not as simple as it looks :) , let me explain
        [] this represents the bytes of memory needed

            What we have                 What we want
        [free_space] + [text]          [logo] + [text]

        But the thing is logo needs more bytes than printed means
                        [free_space] < [logo]
        which implies,  [free_space] + [need] = [logo]

        therefore we reallocate memory to array which looks like
        [free_space] + [text] + [need]

        Now shift the text after need
        [free_space] + [need] + [text]
        [logo] + [text]

        and this is what we want ;)

        parameters in function are:
        need = len(uni_string) - free_space
        but free_space starting from pos_x

    */

    // printf("\n%s\n", uni_string);
    if ((pos_x == 0) || (pos_y == 0)) {
        printf("coordinates lies outside the box\n");
        return;
    }
    if (pos_y > box->len_y - 1) {
        printf("coordinates lies outside the box\n");
        return;
    }
    if (( pos_x + free_space - 1 > *(box->len_x + pos_y) )) {
        printf("coordinates lies outside the box\n");
        return;
    }

    // pos_x is actually from where free_space start
    char *temp_row = *(box->rows + pos_y);
    char *temp;
    char backup = *(temp_row + pos_x + free_space);
    // backing up just next character after uni_string

    // starting point (temp_row + pos_x)

    int len = strlen(uni_string);
    int need = len - free_space;
    if (need == 0) { return; }

    // allocating more space
    temp = realloc(temp_row, *(box->size_x + pos_y) + need);  // we need only extra
    if (temp == NULL) {
        printf("memory cannot be re allocated to row\n");
        return;
    } else {
        temp_row = temp;
    }

    *(box->rows + pos_y) = temp_row; // stored reallocated adress in rows
    int old_row_size = *(box->size_x + pos_y);
    // storing changed length
    *(box->size_x + pos_y) = *(box->size_x + pos_y) + need;

    x_shifter(box, pos_x + free_space, old_row_size - 1, pos_y, need);

    strcpy(temp_row + pos_x, uni_string); // copied unicoded string
    *(temp_row + pos_x + len) = backup;   // removed \0 after copy

    // updating information
    int b1[2][2] = { {pos_x, pos_y}, {pos_x + len, pos_y}};
    int l1[2][2] = { {pos_x, pos_y}, {pos_x + free_space, pos_y}};
    if (element != NULL) {
        int pos = darray_get_pos(box->elements, element);
        if (pos == -1) {
            printf("\nelement not found  %s for ucode\n", element);
            return;
        }

        // update darray loc
        int dbloc[2][2]; int dloc[2][2];
        // can also use get_xy 
        darray_get(box->elements, pos, dloc, dbloc, NULL);
        if (dbloc[1][1] == pos_y) {
            dbloc[1][0] += need; // final x2
        }
        set_bxy(box->elements, pos, dbloc);

        if (*(box->elements->uval + pos) == 0) {
            printf("\nucode not found for %s \n", element);
            return;
        }
        // ucode *uc = gen_ucode();
        // *(box->elements->u + pos) = uc;
        // add ucode check
        ucode_append(*(box->elements->u + pos), b1, l1);
    }
    inc_elements(box, pos_y, pos_x, need);
}

void color_writer(xy_plane *box, int pos_x1, int pos_x2, int pos_y, int color_code) {
    // add color to a row

    pos_x2 = pos_x2 + 1; // for including last char in color
    char color_string[6];
    if (color_code == 0) {
        sprintf(color_string, "\x1b[00m");
    } else {
        sprintf(color_string, "\x1b[%dm", color_code);
    }
    // printf("color_string : %s", color_string);

    char *temp_row = NULL;
    char *temp = NULL;
    temp_row = *(box->rows + pos_y);

    int len = strlen(color_string) ;
    temp = realloc(temp_row, (*(box->size_x + pos_y) + (2 * len + 1))); // + 1 for \0
    if (temp == NULL) {
        printf("memory cannot be re allocated to row\n");
        return;
    } else {
        temp_row = temp;
    }

    *(box->rows + pos_y) = temp_row; // stored reallocated adress in rows
    // storing changed length
    int old_size = *(box->size_x + pos_y);
    *(box->size_x + pos_y) += (2 * (len) );

    char backup1 = *(temp_row + pos_x1); // needed 1st char as strcpy \0
    char backup2 = *(temp_row + pos_x2);

    x_shifter(box, pos_x1, old_size, pos_y, len);

    strcpy(temp_row + pos_x1, color_string);
    *(temp_row + pos_x1 + len) = backup1;  // removed copied \0

    x_shifter(box, pos_x2 + len, old_size + len, pos_y, len);

    strcpy((temp_row + pos_x2 + len), "\x1b[00m"); // reset string at end
    *(temp_row + pos_x2 + len + len) = backup2;

    // *(box->size_x + pos_y) -= 2;

    inc_elements(box, pos_y, pos_x1, 2 * len);
}

/* instead you should actually make a better method for 
detecting colors maybe by size but actual diff [done]

and make a color remove method (working on it) [done]

also update darray->loc of element when unicode or color [done]
*/

int calculate_increment(xy_plane *box, int pos_y, int pos_x1) {
    // calculate extra size (padding) due to unicode
    int inc = 0;
    int bloc[2][2];
    int loc[2][2];
    int *uv = box->elements->uval;
    for (int i = 0; i < box->elements->len; i++) {
        int is_ucode = *(uv + i);
        if (is_ucode == 1) {
            // dont hvae uvalues len using j index
            ucode *temp = *(box->elements->u + i);
            for (int k = 0; k < temp->len; k++) {
                uget_bxy(temp, k, bloc);
                uget_xy(temp , k , loc);
                
                // condition for chekcing if unicode come before
                int flag = (bloc[0][1] == pos_y) && (bloc[0][0] < pos_x1);
                if (flag) {
                    int actual = bloc[1][0] - bloc[0][0];
                    int look = loc[1][0] - loc[0][0];
                    int extra = actual - look;
                    inc += extra;
                    // break; // no need to check further lines
                    if ( *(box->elements->cval + i) == 1) {
                        inc += 2 * 5;
                    }
                }
            }
        } else {
            if ( *(box->elements->cval + i) == 1) {
                get_bxy(box->elements, i, bloc);
                int flag = (bloc[0][1] == pos_y) && (bloc[0][0] < pos_x1);
                if (flag) {
                    inc += 2 * 5;
                }
            }
        }
        
    }
    return inc;
}


void color_by_name(xy_plane *box, char *name, int color_code) {
    // color by name func()
    // int loc[2][2];  // coordinates of element
    int bloc[2][2]; // actual coordinates

    int pos = darray_get_pos(box->elements, name);
    if (pos == -1) {
        printf("\nelement not found  %s for coloring\n", name);
        return;
    }
    int r = *(box->elements->cval + pos);
    if (r == 1) {
        printf("element already have color!!\n");
        return;
    }

    if ( *(box->elements->uval + pos) == 1 ) {
        color_unicode(box, name, color_code);
        return;
    }
    
    get_bxy(box->elements, pos, bloc);


    // position is not always same as unicode change each line length
    for (int j = bloc[0][1]; j < bloc[1][1] + 1; j++) {     
        int xi = bloc[0][0];
        int xf = bloc[1][0]; 
        // need to increment both for ucode
        // printf("color: xi%d xf%d\n", xi,xf);
        int inc = calculate_increment(box, j, xi);
        xi += inc;
        xf += inc;
        // printf("color inc: xi%d xf%d\n", xi,xf);
        color_writer(box, xi, xf, j, color_code);
    }

    // change after adding color
    bloc[0][0] += 5;  // 5 for "\x1b[00m"
    bloc[1][0] += 5;
    set_bxy(box->elements, pos, bloc);

    darray_set_color_flag(box->elements, name, 1);
}

void color_unicode(xy_plane *box, char *name, int color_code) {
    // color an unicode element
    int bloc[2][2]; // actual coordinates

    int pos = darray_get_pos(box->elements, name);
    if (pos == -1) {
        printf("\nelement not found  %s for coloring\n", name);
        return;
    }
    if (*(box->elements->uval + pos) == 0) {
        printf("\nucode not found for  %s \n", name);
        return;
    }
    
    ucode *uc = *(box->elements->u + pos);

    // int loc[2][2];
    // get_xy(box->elements, pos, loc);
    // int start = loc[0][0]; // x1
    // int end = loc[1][0]; // x2

    for (int i = 0; i < uc->len; i++) {

        uget_bxy( uc, i, bloc); 
        // printf("bloc00 %d bloc10 %d bloc01 %d\n", bloc[0][0], bloc[1][0], bloc[0
        // ][1]);

        int xi = bloc[0][0];
        int xf = bloc[1][0];
        // no need to increment as unicode each row ++ when inc_elements
        int j = bloc[0][1];
        color_writer(box, xi, xf, j, color_code);

        // update darray loc
        int dbloc[2][2]; int dloc[2][2];
        // can also use get_xy 
        darray_get(box->elements, pos, dbloc, dloc, NULL);
        if (dbloc[0][1] == bloc[0][1]) {
            dbloc[0][0] = bloc[0][0] + 5; // final x1
        }
        if (dbloc[1][1] == bloc[0][1]) {
            dbloc[1][0] = bloc[1][0] + 5; // final x2
        }
        set_bxy(box->elements, pos, dbloc);

        // change after adding color
        bloc[0][0] += 5;  // 5 for "\x1b[00m"
        bloc[1][0] += 5;
        uset_bxy( uc , i, bloc);
    }

    darray_set_color_flag(box->elements, name, 1);
}

int check_color_by_name(xy_plane *box, char *element) {
    // -2 error return -1 if no color else color code length
    int bloc[2][2]; int loc[2][2];
    int pos = darray_get_pos(box->elements, element);
    if (pos == -1) {
        printf("check color element not found!\n");
        return -2;
    }
    darray_get(box->elements, pos, bloc, loc, NULL);
    
    int diff = bloc[0][0] - loc[0][0];
    if (diff == 5 || diff == 4) {
        return 5;
    }
    return -1;
}

void decolor(xy_plane *box, int pos_x1, int pos_x2, int pos_y) {
    // remove color from already added string
    char *temp_row, *temp;
    temp_row = *(box->rows + pos_y);

    int code_len = 5; // "\x033[33m"

    pos_x2 = pos_x2 + 1; // for including last char

    // removing first color code
    x_shifter(box, pos_x1, *(box->size_x + pos_y), pos_y, -1 * (code_len));

    // removing last color code
    x_shifter(box, pos_x2, *(box->size_x + pos_y), pos_y, -1 * (code_len));
    
    
    // changing final size_x
    temp = realloc(temp_row, (*(box->size_x + pos_y) - (2 * code_len)));
    if (temp == NULL) {
        printf("memory cannot be reduced from a row\n");
        return;
    } else {
        temp_row = temp;
    }

    *(box->rows + pos_y) = temp_row; // stored reallocated adress in rows
    
    // storing changed length
    *(box->size_x + pos_y) -= 2 * (code_len);

    inc_elements(box, pos_y, pos_x1, -1 * (2 * code_len)); 
}

void decolor_unicode(xy_plane *box, char *name) {
    int bloc[2][2]; // actual coordinates

    int pos = darray_get_pos(box->elements, name);
    if (pos == -1) {
        printf("\nelement not found  %s for removing color\n", name);
        return;
    }
    if (*(box->elements->uval + pos) == 0) {
        printf("\nucode not found for  %s \n", name);
        return;
    }
    
    ucode *uc = *(box->elements->u + pos);
    for (int i = 0; i < uc->len; i++) {

        uget_bxy( uc, i, bloc); 
        // printf("bloc00 %d bloc10 %d bloc01 %d\n", bloc[0][0], bloc[1][0], bloc[0
        // ][1]);

        int xi = bloc[0][0];
        int xf = bloc[1][0];
        // no need to increment as unicode each row ++ when inc_elements
        int j = bloc[0][1];
        decolor(box, xi, xf, j);

        // update darray loc
        int dbloc[2][2]; int dloc[2][2];
        // can also use get_xy 
        darray_get(box->elements, pos, dbloc, dloc, NULL);
        if (dbloc[0][1] == bloc[0][1]) {
            dbloc[0][0] = bloc[0][0] - 5; // final x1
        }
        if (dbloc[1][1] == bloc[0][1]) {
            dbloc[1][0] = bloc[1][0] - 5; // final x2
        }
        set_bxy(box->elements, pos, dbloc);

        // change after adding color
        bloc[0][0] -= 5;  // 6 for "\x1b[00m"
        bloc[1][0] -= 5;
        uset_bxy( uc , i, bloc);
    }

    darray_set_color_flag(box->elements, name, 0);

}

void decolor_by_name(xy_plane *box, char *name) {
    // check color

    int bloc[2][2]; // actual coordinates

    int pos = darray_get_pos(box->elements, name);
    if (pos == -1) {
        printf("\nelement not found  %s for coloring\n", name);
        return;
    }
    int r = *(box->elements->cval + pos);
    if (r != 1) {
        printf("decolor: element dosen't have color!!\n");
        return;
    }

    if ( *(box->elements->uval + pos) == 1 ) {
        decolor_unicode(box, name);
        return;
    }

    get_bxy(box->elements, pos, bloc);

    // position is not always same as unicode change each line length
    for (int j = bloc[0][1]; j < bloc[1][1] + 1; j++) {
        int xi = bloc[0][0];
        int xf = bloc[1][0];
        // printf("decolor: xi%d xf%d\n", xi,xf);
        int inc = calculate_increment(box, j, xi);
        xi += inc;
        xf += inc;
        // printf("decolor inc: xi%d xf%d\n", xi,xf);
        decolor(box, xi, xf, j);
    }

    // change after adding color
    bloc[0][0] -= 5;  // 5 for "\x1b[00m"
    bloc[1][0] -= 5;
    set_bxy(box->elements, pos, bloc);

    darray_set_color_flag(box->elements, name, 0);
}

void element_fill(xy_plane *box, char *name, char c) {
    // fill with a character
    int bloc[2][2]; int loc[2][2];
    int pos = darray_get_pos(box->elements, name);
    if (pos == -1) {
        printf("\nelement not found  %s for filling\n", name);
        return;
    }
    
    darray_get(box->elements, pos, bloc, loc, NULL);

    int len = bloc[1][0] - bloc[0][0] + 1;
    char str[len + 1];
    for (int i = 0; i < len; i++) {
        str[i] = c;
    }
    str[len] = '\0';

    for (int j = bloc[0][1]; j < bloc[1][1] + 1; j++) {
        write_line(box, bloc[0][0], bloc[1][0] + 1, j, str);
    }
}


/*
1. use lenx or leny for box coordinates and use size_x for actual bytes including 4 [done]
*/