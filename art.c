#include <stdlib.h>
#include <string.h>
#include "art.h"

int art_width = 19;
int art_height = 7;

char*** gen_art() {
    char ***arts = malloc(sizeof (char **) * 6);
    *(arts + 0) = l_rock;
    *(arts + 1) = l_paper;
    *(arts + 2) = l_scissor;
    *(arts + 3) = r_rock;
    *(arts + 4) = r_paper;
    *(arts + 5) = r_scissor;

    for (int i = 0; i < 6; i++) {
        char **tt = malloc(sizeof (char *) * art_height);
        for (int j = 0; j < art_height; j++) {
            char *t = malloc(sizeof (char) * art_width);
            strcpy(t, *(*(arts + i) + j));
            *(tt + j) = t;
        }
        *(arts + i) = tt;
    }
    return arts;
}

struct art_couple get_art(int player, int computer, char ***arts) {
    // o rock 1 paper 2 scissor

    char **left;
    if (player == 0) {
        left = *(arts + 0);
    }
    else if (player == 1) {
        left = *(arts + 1);
    }
    else if (player == 2) {
        left = *(arts + 2);
    }

    char **right;
    if (computer == 0) {
        right = *(arts + 3);
    }
    else if (computer == 1) {
        right = *(arts + 4);
    }
    else if (computer == 2) {
        right = *(arts + 5);
    }

    struct art_couple a = {left, right};
    return a;
}

void free_arts(char ***arts) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < art_height; j++) {
            free( *(*(arts + i) + j) );
        }
        free( *(arts + i) );
    }
    free(arts);
}
