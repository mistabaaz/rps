#ifndef ART_H
#define ART_H

char*** gen_art();  // convert static art into dynamic
void free_arts(char ***arts);

struct art_couple {
    char **left;
    char **right;
};

struct art_couple get_art(int player, int computer, char ***arts);

/* THANKS TO wynand1004 , Harleythetech (for reverse)*/

// every icon is 19x7 size
extern int art_width;
extern int art_height;

static char *l_rock[] = {
    "    _______       ",
    "---'   ____)      ",
    "      (_____)     ",
    "      (_____)     ",
    "      (____)      ",
    "---.__(___)       ",
    "ROCK              "
};

static char *l_paper[] = {
    "     _______      ",
    "---'    ____)____ ",
    "           ______)",
    "          _______)",
    "         _______) ",
    "---.__________)   ",
    "PAPER             "
};

static char *l_scissor[] = {
    "    _______       ",
    "---'   ____)____  ",
    "          ______) ",
    "      __________) ",
    "      (____)      ",
    "---.__(___)       ",
    "SCISSORS          "
};

static char *r_rock[] = {
    "       _______    ",
    "      (____   '---",
    "     (_____)      ",
    "     (_____)      ",
    "      (____)      ",
    "       (___)__.---",
    "              ROCK"
};

static char *r_paper[] = {
    "      _______     ",
    " ____(____   '----",
    "(______           ",
    "(_______          ",
    " (_______         ",
    "   (__________.---",
    "             PAPER"
};

static char *r_scissor[] = {
    "       _______    ",
    "  ____(____   '---",
    " (______          ",
    " (__________      ",
    "      (____)      ",
    "       (___)__.---",
    "          SCISOORS"
};

// art matrix to
const static int art[3][3] = {
    {2, 0, 1}, // 0 means user lose
    {1, 2, 0}, // 1 means user win
    {0, 1, 2}  // 2 means draw
};

#endif

