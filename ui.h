#ifndef UI_H
#define UI_H

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#ifndef DISABLE_NEWLINE_AUTO_RETURN
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008
#endif

typedef struct xy_plane {
    char **rows;  // adress of strings
    int *size_x;  // depends on y-axis eg. [2,2,2,2]
    int size_y;  // actual size in bytes
    int *len_x;  // length of characters displayed
    int len_y;  // how much character space it takes on screen
    darray *elements;  // pointer to elements name & loc
} xy_plane;

struct ascii_art {
    char *name;
    char *art;
};

int comp_choice();
int run();

xy_plane* gen_xyplane();
void create_box(xy_plane *box);
void print_box(xy_plane *box);
void free_box(xy_plane *box);
void embed_box(xy_plane *box, int loc[2][2], char *element);
void write_line(xy_plane *box, int pos_x1, int pos_x2, int pos_y, char *string); 
void clear_lines(int lines, int backward, int all, int one);
void move_cursor(int up_down, int right_left);
void unicode_writer(xy_plane *box, int pos_y, int pos_x, char *uni_string, int free_space, char *element);
void color_writer(xy_plane *box, int pos_x1, int pos_x2, int pos_y, int color_code);
void array_shift(char arr[], int arr_size, int shift_by, int text_len);
void color_by_name(xy_plane *box, char *name, int color_code);
void color_unicode(xy_plane *box, char *name, int color_code);
void decolor(xy_plane *box, int pos_x1, int pos_x2, int pos_y);
void decolor_by_name(xy_plane *box, char *name);
int enable_terminal_settings(int size_x, int size_y);
void element_fill(xy_plane *box, char *name, char c);
int calculate_increment(xy_plane *box, int pos_y, int pos_x1); 

#endif