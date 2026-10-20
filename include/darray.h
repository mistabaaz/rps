#ifndef DARRAY_H
#define DARRAY_H


// for storing len of unicode chars
typedef struct unicode {
    int ***bloc; // actual bytes used array
    int ***loc;  // location array loc[2][2]
    int len;
    int size;
} ucode;

// dynamic array for storing string (names of elements)
struct darray{
    char **names;  // names of element
    int ***bloc;  // position of elements in { {x1,y1}, {x2,y2} }
    int ***loc;  // position of elements in { {x1,y1}, {x2,y2} }
    int size;  // allocated array size
    int len;  // no of elements
    int apos; // position for appending
    int lpos; // position of last element
    int *uval; // if ucode avail
    int *cval; // color flag
    int *values; // allocated names (memory) have value or not
    ucode **u;  // ucode struct poiner loc if unicode
};
typedef struct darray darray;

darray* gen_darray();
void darray_gen_space(darray *array, int size);
void darray_set(darray *array, int pos, char *element, int bloc[2][2], int loc[2][2], ucode *uc);
void darray_insert(darray *array, int pos, char *element, int bloc[2][2], int loc[2][2], ucode *uc);
char* darray_get(darray *array, int pos, int bloc[2][2], int loc[2][2], ucode *uc);
int darray_get_pos(darray *array, char *element);
int darray_get_loc_by_name(darray *array, char *element, int bloc[2][2], int loc[2][2]);
int darray_realloc(darray *array);
void darray_shift(darray *array, int shift_by, int shift_from);
void darray_append(darray *array, char *element, int bloc[2][2], int loc[2][2], ucode *uc);
void darray_remove(darray *array, char *element);
void darray_pop(darray *array, int pos);
void darray_update_name(darray *array, char *prev, char *next);
void darray_update_loc(darray *array, int pos, int bloc[2][2], int loc[2][2]);
void darray_update_loc_by_name(darray *array, char *element, int bloc[2][2], int loc[2][2]);
void darray_replace(darray *array, int pos, char *element, int bloc[2][2], int loc[2][2]);
void darray_replace_name(darray *array, int pos, char *element);
void darray_exchange(darray *array, char *e1, char *e2);
void darray_swap(darray *array, int p1, int p2);
void darray_clear(darray *array);
void darray_free(darray *array);
void darray_show_json(darray *array);
char* darray_json_string(darray *array);
int darray_json_file(darray *array);
void darray_set_color_flag(darray *array, char *element, int value);
int darray_get_color_flag(darray *array, char *element);

ucode* gen_ucode();
void ucode_append(ucode *u, int bloc[2][2], int loc[2][2]);
void ucode_show_json(ucode *uc);
void ucode_free(ucode *u);
char* ucode_json_string(ucode *uc);
int ucode_json_file(ucode *uc);

int ud_json_file(darray *array); // add both darray & ucode in json

/* not so optimized functions */
int** gen_xy(int loc[2][2]);
void get_xy(darray *array, int pos, int loc[2][2]);
void set_xy(darray *array, int pos, int loc[2][2]);
void get_bxy(darray *array, int pos, int bloc[2][2]);
void set_bxy(darray *array, int pos, int bloc[2][2]);

void uget_bxy(ucode *uc,int pos, int bloc[2][2]);
void uset_bxy(ucode *uc,int pos, int bloc[2][2]);
void uget_xy(ucode *uc,int pos, int loc[2][2]);
void uset_xy(ucode *uc,int pos, int loc[2][2]);

#endif
