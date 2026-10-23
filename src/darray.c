#include <stdio.h>    // printf(), fprintf()
#include <stdlib.h>  // malloc(), realloc()
#include <string.h> // strcpy(), strlen()
#include "darray.h"

// dynamic array for storing string (names of elements)


darray* gen_darray() {
    /* create dynamic array */
    int size = 1;
    int len = 0;
    int apos = 0;
    int lpos = 0;
    char **names = malloc(sizeof (char*) * size);
    int ***bloc = malloc(sizeof (int **) * size); 
    int ***loc = malloc(sizeof (int **) * size);
    int *uval = malloc(sizeof (int) * size);
    int *cval = malloc(sizeof (int) * size);
    int *values = malloc(sizeof (int) * size);
    if (names == NULL || bloc == NULL || loc == NULL || values == NULL || uval == NULL || cval == NULL) {
        printf("\nnames or loc or values or cval or uval memory not allocated!\n");
        return NULL;
    }
    ucode **u = malloc(sizeof (ucode *) * size);
    if (u == NULL) {
        printf("\nucode memory not allocated\n");
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        *(uval + i) = 0;
        *(cval + i) = 0;
        *(values + i) = 0; // element memory allocated but value not assigned yet
    }
    darray *array = malloc(sizeof (darray));
    array->names = names;
    array->bloc = bloc;
    array->loc = loc;
    array->size = size;
    array->len = len;
    array->apos = apos;
    array->lpos = lpos;
    array->uval = uval;
    array->cval = cval;
    array->values = values;
    array->u = u;
    return array;
}

void darray_gen_space(darray *array, int size) {
    // allocate memory without values
    int new_size = array->size + size;
    char **n = realloc(array->names, sizeof(char*) * new_size);
    int ***b = realloc(array->bloc, sizeof(int**) * new_size);
    int ***l = realloc(array->loc, sizeof(int**) * new_size);
    int *uv = realloc(array->uval, sizeof(int) * new_size);
    int *cv = realloc(array->cval, sizeof(int) * new_size);
    int *v = realloc(array->values, sizeof(int) * new_size);
    ucode **u = realloc(array->u, sizeof(ucode*) * new_size);
    if (n == NULL || b == NULL || l == NULL || v == NULL || u == NULL || uv == NULL || cv == NULL) {
        printf("\nnames or loc memory not allocated!\n");
        return;
    }
    for (int i = array->size; i < new_size; i++) {
        *(uv + i) = 0;
        *(cv + i) = 0;
        *(v + i) = 0; // element memory allocated but value not assigned yet
    }
    array->names = n;
    array->bloc = b;
    array->loc = l;
    array->size = new_size;
    array->uval = uv;

    array->cval = cv;
    array->values = v;
    array->u = u;
    return;
}

// gen_xy(), get_xy(), set_xy() functions needed [done]

void darray_set(darray *array, int pos, char *element, int bloc[2][2], int loc[2][2], ucode *uc) {
    // set value to unused darray space
    if (pos > array->size - 1) {
        printf("pos=%d not availible in allocated space=%d\n", pos, array->size - 1);
        return;
    } else if (*(array->values + pos) != 0) {
        printf("space already occupied means value already there at pos=%d\n", pos);
        return;
    } else {
        char *name = malloc(strlen(element) + 1);
        if (name == NULL) {
            printf("\n memory not allocated for name\n");
            return;
        }
        strcpy(name, element);

        int **bxy = gen_xy(bloc);
        int **xy = gen_xy(loc);
        if (bxy == NULL || xy == NULL) {
            printf("\n memory not allocated for xy\n");
            return;
        }

        *(array->names + pos) = name;
        *(array->bloc + pos) = bxy; // added xy cordinate pair(array) in loc
        *(array->loc + pos) = xy; // added xy cordinate pair(array) in loc
        array->len = array->len + 1;
        *(array->values + pos) = 1;  // value used (position occupied)
        if (pos > array->lpos) {
            array->lpos = pos;
        }
        if (uc == NULL) {
            *(array->uval + pos) = 0;
        }
        *(array->u + pos) = uc; // if NULL check it
    }
}

void darray_insert(darray *array, int pos, char *element, int bloc[2][2], int loc[2][2], ucode *uc) {
    // if pos availible (set value) otherwise shift array
    if (pos > array->size - 1) {
        printf("pos=%d not availible in allocated space=%d\n", pos, array->size - 1);
        return;
    } else if (pos > array->lpos) {
        darray_set(array, pos, element, bloc, loc, uc);
        return;
    } else {
        char *name = malloc(strlen(element) + 1);
        if (name == NULL) {
            printf("\n memory not allocated for name\n");
            return;
        }
        strcpy(name, element);

        int **bxy = gen_xy(bloc);
        int **xy = gen_xy(loc);
        if (bxy == NULL || xy == NULL) {
            printf("\n memory not allocated for xy\n");
            return;
        }

        darray_shift(array, 1, pos);
        *(array->names + pos) = name;
        *(array->bloc + pos) = bxy; // added xy cordinate pair(array) in loc
        *(array->loc + pos) = xy; // added xy cordinate pair(array) in loc
        array->len = array->len + 1;
        *(array->values + pos) = 1;
        if (uc != NULL) {
            *(array->uval + pos) = 1;
        }
        *(array->u + pos) = uc;
        return;
    }
}

char* darray_get(darray *array, int pos, int bloc[2][2], int loc[2][2], ucode *uc) {
    // get name of element & location loc[2][2]
    char *name;
    if (pos > array->size - 1) {
        printf("\nindex=%d is out of array size=%d\n", pos, array->size - 1);
        return NULL;
    } else if (*(array->values + pos) != 1) {
        printf("\nindex=%d dosen't have any value\n", pos);
        return NULL;
    } else {
        get_bxy(array, pos, bloc);
        get_xy(array, pos, loc);
        name = *(array->names + pos);
        if (*(array->uval + pos) == 1 && uc != NULL) {
            uc = *(array->u + pos); // return ucode pointer
        } else {
            uc = NULL;
        }
        return name;
    }
}

int darray_get_pos(darray *array, char *element) {
    // return location of darray element loc[2][2]
    char *name; int found = 0; int pos=-1;
    for (int i = 0; i < array->lpos + 1; i++) {
        if (*(array->values + i) == 1) {
            name = *(array->names + i);
            if ( strcmp(element, name) == 0 ) {
                found = 1;
                pos = i;
            }
        }
    }
    if (found == 1) {
        return pos;
    } else {
        return -1;
    }
}

int darray_get_loc_by_name(darray *array, char *element, int bloc[2][2], int loc[2][2]) {
    // return loc[2][2] by element name
    int pos = darray_get_pos(array, element);
    if (pos == -1) {
        printf("\n.. element=%s not exist\n", element);
        return - 1;
    }
    get_bxy(array, pos, bloc);
    get_xy(array, pos, loc);
    return pos;
}

int darray_realloc(darray *array) {
    // it doubles the darray size
    char **temp; int ***b; int ***l; int *v; int *uv; int *cv;
    ucode **u;
    temp = realloc(array->names, sizeof (char *) * array->size * 2); // dynamically double the size
    b = realloc(array->bloc, sizeof (int **) * array->size * 2);
    l = realloc(array->loc, sizeof (int **) * array->size * 2);
    uv = realloc(array->uval, sizeof (int) * array->size * 2);
    cv = realloc(array->cval, sizeof (int) * array->size * 2);
    v = realloc(array->values, sizeof (int) * array->size * 2);
    u = realloc(array->u, sizeof (ucode **) * array->size * 2);
    if (temp == NULL || b == NULL || l == NULL || v == NULL || u == NULL || uv == NULL || cv == NULL) {
        printf("\nnames or loc or values memory cannot be reallocated for shifting\n");
        return 1;
    }
    for (int i = array->size; i < array->size * 2; i++) {
        *(uv + i) = 0;
        *(cv + i) = 0;
        *(v + i) = 0; // element memory allocated but value not assigned yet
    }
    array->names = temp;
    array->bloc = b;
    array->loc = l;
    array->size = array->size * 2;
    array->uval = uv;
    array->cval = cv;
    array->values = v;
    array->u = u;

    return 0;
}

void darray_shift(darray *array, int shift_by, int shift_from)
{
    /* shift the text of length (text_len) by (shift_by)
    + means forword shift - means backward shift */

    if (array->len + shift_by >= array->size) {   
        // means need more free space
        int err = darray_realloc(array);
        if (err != 0) {
            printf("\nerror reallocating darray memory\n");
            return;
        }
    }

    if (shift_from < 0 || shift_from > array->lpos - 1) {
        printf("can't shift elements out of array boundries\n");
        return;
    }

    if (shift_by < 0) {
        // removing elements before shifting if not removed
        for (int i = shift_from + shift_by; i < shift_from; i++) {
            if (*(array->values + i) == 1) {
                printf("element is removed due to backward shifting!!\n");
                fflush(stdout);
                free(*(array->names + i));

                // freeing xy blocks bloc & loc
                free(*(*(array->bloc + i) + 0));  // free {x1,y1}
                free(*(*(array->bloc + i) + 1));
                free(*(array->bloc + i));  // // free location of element

                free(*(*(array->loc + i) + 0));  // free {x1,y1}
                free(*(*(array->loc + i) + 1));
                free(*(array->loc + i));  // // free location of element

                if (*(array->u + i) != NULL) {
                    free(*(array->u + i));  // freed ucode
                }
                
                *(array->uval + i) = 0;
                *(array->cval + i) = 0;
                *(array->values + i) = 0;
                array->len--;
            }
        }
        // means backward shifting
        shift_by = shift_by * -1;
        for (int i = 0; i < shift_by; i++)
        {
            for (int j = shift_from - i; j < array->lpos - i + 1; j++)
            {
                // copying next element to previous position
                *(array->names + j - 1) = *(array->names + j);
                *(array->bloc + j - 1) = *(array->bloc + j);
                *(array->loc + j - 1) = *(array->loc + j);
                *(array->uval + j - 1) = *(array->uval + j);
                *(array->cval + j - 1) = *(array->cval + j);
                *(array->values + j - 1) = *(array->values + j);
                *(array->u + j - 1) = *(array->u + j);
                
            }
        }
        // freeing up duplicate values at last
        for (int i = array->lpos; i > array->lpos - shift_by; i--) {
            *(array->uval + i) = 0;
            *(array->cval + i) = 0;
            *(array->values + i) = 0; // elements at last dosen't have values now
            // array->apos--;
        }
        array->lpos = array->lpos - shift_by;  // element removed from last position
        if (array->apos > shift_from) {
            array->apos = array->apos - shift_by;
        }
    } else {
        // means forword shifting
        for (int i = 0 ; i < shift_by; i++)
        {
            for (int j = array->lpos + i; j > shift_from + i - 1; j--)
            {
                // copying last element to next position
                *(array->names + j + 1) = *(array->names + j);
                *(array->bloc + j + 1) = *(array->bloc + j);
                *(array->loc + j + 1) = *(array->loc + j);
                *(array->uval + j + 1) = *(array->uval + j);
                *(array->cval + j + 1) = *(array->cval + j);
                *(array->values + j + 1) = *(array->values + j);
                *(array->u + j + 1) = *(array->u + j);

            }
        }
        // freeing up duplicate values in between 
        for (int i = shift_from ; i < shift_from + shift_by; i++) {
            *(array->uval + i) = 0;
            *(array->cval + i) = 0;
            *(array->values + i) = 0; // element memory allocated but value not assigned yet
        }
        array->lpos = array->lpos + shift_by; // last element position increased
    }
    return;

}
                                   
void darray_append(darray *array, char *element, int bloc[2][2], int loc[2][2], ucode *uc) {
    // add element to last continous availible position [apos]
    if (array->len >= array->size) {
        // means need more free space
        int err = darray_realloc(array);
        if (err != 0) {
            printf("\nerror reallocating darray memory\n");
            return;
        }
    }
    for (int i = array->apos; i < array->lpos + 2; i++) {
        // i forgot why for loop?
        /* ans if last postition is something else 
           which can be if element added by insert
           then we have to traverse through last
           for free pos.
        */
        if (*(array->values + i) != 0) {
            printf("i = %d, arr value : %d\n", i, *(array->values + i));
            // element already occupied
            printf(" 2 element already occupied\n");
        } else {
            char *name = malloc(strlen(element) + 1);
            if (name == NULL) {
                printf("\n memory not allocated for name\n");
                return;
            }
            strcpy(name, element);

            int **bxy = gen_xy(bloc);
            int **xy = gen_xy(loc);
            if (bxy == NULL || xy == NULL) {
                printf("\n memory not allocated for xy(darray_append)\n");
                return;
            }

            *(array->names + i) = name;
            *(array->bloc + i) = bxy; // added xy cordinate pair(array) in loc
            *(array->loc + i) = xy; // added xy cordinate pair(array) in loc
            array->len = array->len + 1;
            array->apos = i + 1;  // stored next pos in apos
            *(array->values + i) = 1;
            if (i > array->lpos) {
                array->lpos = i; // last element position updated
            }
            if (uc != NULL) {
                *(array->uval + i) = 1;
            }
            *(array->u + i) = uc;
            return;
        }
    }
    
}

void darray_remove(darray *array, char *element) {
    // remove element by name
    int pos = darray_get_pos(array, element);
    if (pos == -1) {
        printf("\nremoving failed.. element=%s not exist\n", element);
        return;
    }
    free(*(array->names + pos));

    // freeing xy blocks bloc & loc
    free(*(*(array->bloc + pos) + 0));  // free {x1,y1}
    free(*(*(array->bloc + pos) + 1));
    free(*(array->bloc + pos));  // // free location of element

    free(*(*(array->loc + pos) + 0));  // free {x1,y1}
    free(*(*(array->loc + pos) + 1));
    free(*(array->loc + pos));  // // free location of element

    if (*(array->u + pos) != NULL) {
        free(*(array->u + pos));
    }

    *(array->uval + pos) = 0;
    *(array->cval + pos) = 0;
    *(array->values + pos) = 0;
    darray_shift(array, -1, pos + 1);
    array->len = array->len - 1;
    return;
}

void darray_pop(darray *array, int pos) {
    // remove element by pos (in array)
    if (pos > array->size - 1) {
        printf("\nindex=%d is out of array size=%d\n", pos, array->size - 1);
        return;
    } else if (*(array->values + pos) != 1) {
        printf("\nindex=%d is dosen't have any value\n", pos);
        return;
    } else {
        free(*(array->names + pos));
 
        // freeing xy blocks bloc & loc
        free(*(*(array->bloc + pos) + 0));  // free {x1,y1}
        free(*(*(array->bloc + pos) + 1));
        free(*(array->bloc + pos));  // // free location of element

        free(*(*(array->loc + pos) + 0));  // free {x1,y1}
        free(*(*(array->loc + pos) + 1));
        free(*(array->loc + pos));  // // free location of element

        if ( *(array->u + pos) !=NULL ) {
            free(*(array->u + pos));
        }

        *(array->uval + pos) = 0;
        *(array->cval + pos) = 0;
        *(array->values + pos) = 0;
        darray_shift(array, -1, pos + 1);
        array->len = array->len - 1;
        return;
    }
}

void darray_update_name(darray *array, char *prev, char *next) {
    // set new name of element
    int pos = darray_get_pos(array, prev);
    if (pos == -1) {
        printf("\nupdating failed.. element=%s not exist\n", prev);
        return;
    }
    char *temp = malloc(strlen(next) + 1);
    if (temp == NULL) {
        printf("\n memory not allocated for name\n");
        return;
    }
    strcpy(temp, next);
    free(*(array->names + pos));
    *(array->names + pos) = temp;
    return;
}

void darray_update_loc(darray *array, int pos, int bloc[2][2], int loc[2][2]) {
    // update loc[2][2] by index(in array)
    if (pos > array->size - 1) {
        printf("\nindex=%d is out of array size=%d\n", pos, array->size - 1);
        return;
    } else if (*(array->values + pos) != 1) {
        printf("\nindex=%d is dosen't have any value\n", pos);
        return;
    } else {
        set_bxy(array, pos, bloc);
        set_xy(array, pos, loc);
        return;
    }
}

void darray_update_loc_by_name(darray *array, char *element, int bloc[2][2], int loc[2][2]) {
    // update loc[2][2] by name
    int pos = darray_get_pos(array, element);
    if (pos == -1) {
        printf("\nupdating failed.. element=%s not exist\n", element);
        return;
    }
    set_bxy(array, pos, bloc);
    set_xy(array, pos, loc);
    return;
}

void darray_update_ucode(darray *array, int pos, ucode *uc) {
    // update ucode ptr using pos
    if (pos > array->size - 1) {
        printf("\nindex=%d is out of array size=%d\n", pos, array->size - 1);
        return;
    } else if (*(array->values + pos) != 1) {
        printf("\nindex=%d is dosen't have any value\n", pos);
        return;
    } else {
        if ( *(array->u + pos) !=NULL ) {
            free(*(array->u + pos));
        }
        if (uc != NULL) {
            *(array->uval + pos) = 1;
        }
        *(array->u + pos) = uc;
        return;
    }
}

void darray_update_ucode_by_name(darray *array, char *element, ucode *uc) {
    // update ucode ptr by name
    int pos = darray_get_pos(array, element);
    if (pos == -1) {
        printf("\nupdating failed.. element=%s not exist\n", element);
        return;
    }
    if ( *(array->u + pos) != NULL) {
        free(*(array->u + pos));
    }
    if (uc != NULL) {
        *(array->uval + pos) = 1;
    }
    *(array->u + pos) = uc;
    return;
}

void darray_set_color_flag(darray *array, char *element, int value) {
    // set color to 0 or 1
    int pos = darray_get_pos(array, element);
    if (pos == -1) {
        printf("\ncolor set failed.. element=%s not exist\n", element);
        return;
    }
    *(array->cval + pos) = value;
    return;
}

int darray_get_color_flag(darray *array, char *element) {
    // set color to 0 or 1
    int pos = darray_get_pos(array, element);
    if (pos == -1) {
        printf("\ncant get color flag.. element=%s not exist\n", element);
        return -1;
    }
    int value = *(array->cval + pos);
    return value;
}

void darray_replace(darray *array, int pos, char *element, int bloc[2][2], int loc[2][2]) {
    // update name & loc by index(in array)
    if (pos > array->size - 1) {
        printf("\nindex=%d is out of array size=%d\n", pos, array->size - 1);
        return;
    } else if (*(array->values + pos) != 1) {
        printf("\nindex=%d is dosen't have any value\n", pos);
        return;
    } else {
        char *temp = malloc(strlen(element) + 1);
        if (temp == NULL) {
            printf("\n memory not allocated for name\n");
            return;
        }
        strcpy(temp, element);  // replaced with new name
        set_bxy(array, pos, bloc);
        set_xy(array, pos, loc); 
        free(*(array->names + pos));
        *(array->names + pos) = temp;
        return;
    }
}

void darray_replace_name(darray *array, int pos, char *element) {
    // update name by index (in array)
    if (pos > array->size - 1) {
        printf("\nindex=%d is out of array size=%d\n", pos, array->size - 1);
        return;
    } else if (*(array->values + pos) != 1) {
        printf("\nindex=%d is dosen't have any value\n", pos);
        return;
    } else {
        char *temp = malloc(strlen(element) + 1);
        if (temp == NULL) {
            printf("\n memory not allocated for name\n");
            return;
        }
        strcpy(temp, element);  // replaced with new name
        free(*(array->names + pos));
        *(array->names + pos) = temp;
        return;
    }
}

void darray_exchange(darray *array, char *e1, char *e2) {
    // exchange elements by names
    int p1 = darray_get_pos(array, e1);
    int p2 = darray_get_pos(array, e2);
    if (p1 == -1) {
        printf("\nexchange failed.. element1=%s not exist\n", e1);
        return;
    } else if (p2 == -1) {
        printf("\nexchange failed.. element2=%s not exist\n", e2);
        return;        
    }
    // firsts swaping element names
    char *temp = *(array->names + p1);
    *(array->names + p1) = *(array->names + p2);
    *(array->names + p2) = temp;
    // now swapping cordinates
    int **b = *(array->bloc + p1);
    *(array->bloc + p1) = *(array->bloc + p2);
    *(array->bloc + p2) = b;
    int **l = *(array->loc + p1);
    *(array->loc + p1) = *(array->loc + p2);
    *(array->loc + p2) = l;

    int uv = *(array->uval + p1);
    *(array->uval + p1) = *(array->uval + p2);
    *(array->uval + p2) = uv;

    int cv = *(array->cval + p1);
    *(array->cval + p1) = *(array->cval + p2);
    *(array->cval + p2) = cv;

    ucode *t = *(array->u + p1);
    *(array->u + p1) = *(array->u + p2);
    *(array->u + p2) = t;
    return;
}

void darray_swap(darray *array, int p1, int p2) {
    // exhange elements by pos(in array)
    if (p1 > array->size - 1) {
        printf("\nindex=%d is out of array size=%d\n", p1, array->size - 1);
        return;
    } else if (p2 > array->size - 1) {
        printf("\nindex=%d is out of array size=%d\n", p2, array->size - 1);
        return;
    } else if (*(array->values + p1) != 1) {
        printf("\nindex=%d is dosen't have any value\n", p1);
        return;
    } else if (*(array->values + p2) != 1) {
        printf("\nindex=%d is dosen't have any value\n", p2);
        return;
    } else {
        // firsts swaping element names
        char *temp = *(array->names + p1);
        *(array->names + p1) = *(array->names + p2);
        *(array->names + p2) = temp;
        // now swapping cordinates
        int **b = *(array->bloc + p1);
        *(array->bloc + p1) = *(array->bloc + p2);
        *(array->bloc + p2) = b;
        int **l = *(array->loc + p1);
        *(array->loc + p1) = *(array->loc + p2);
        *(array->loc + p2) = l;

        int uv = *(array->uval + p1);
        *(array->uval + p1) = *(array->uval + p2);
        *(array->uval + p2) = uv;

        int cv = *(array->cval + p1);
        *(array->cval + p1) = *(array->cval + p2);
        *(array->cval + p2) = cv;

        ucode *t = *(array->u + p1);
        *(array->u + p1) = *(array->u + p2);
        *(array->u + p2) = t;
        return;
    }
}    

void darray_clear(darray *array) {
    // clear elments [free element name memory & set value=0]
    for (int i = 0; i < array->lpos + 1; i++) {
        if (*(array->values + i) == 1) {
            free(*(array->names + i));

            // freeing xy blocks bloc & loc
            free(*(*(array->bloc + i) + 0));  // free {x1,y1}
            free(*(*(array->bloc + i) + 1));
            free(*(array->bloc + i));  // // free location of element

            free(*(*(array->loc + i) + 0));  // free {x1,y1}
            free(*(*(array->loc + i) + 1));
            free(*(array->loc + i));  // // free location of element

            *(array->cval + i) = 0;
            *(array->values + i) = 0;
        }
        if (*(array->uval + i) == 1 && *(array->u + i) != NULL) {
            ucode_free(*(array->u + i));
            *(array->u + i) = NULL;
            *(array->uval + i) = 0;
        }
    }
    array->len = 0;
    array->apos = 0;
    array->lpos = 0;
    return;
}

void darray_free(darray *array) {
    // delete the darray
    for (int i = 0; i < array->lpos + 1; i++) {
        if (*(array->values + i) == 1) {
            free(*(array->names + i));

            // freeing xy blocks bloc & loc
            free(*(*(array->bloc + i) + 0));  // free {x1,y1}
            free(*(*(array->bloc + i) + 1));
            free(*(array->bloc + i));  // // free location of element

            free(*(*(array->loc + i) + 0));  // free {x1,y1}
            free(*(*(array->loc + i) + 1));
            free(*(array->loc + i));  // // free location of element
        }
        if (*(array->uval + i) == 1) {
            ucode_free(*(array->u + i));
        }
    }
    free(array->names);
    free(array->bloc);
    free(array->loc);
    free(array->uval);
    free(array->cval);
    free(array->values);
    free(array->u);
    free(array);
    return;
}

void darray_show_json(darray *array) {
    // print line by line in json format
    if (array->len > 0) {
        printf("\n{\n    \"names\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            if (*(array->values + i) !=0 ) {
                printf("\"%s\", ", *(array->names + i));
            } else {
                printf("null, ");
            }
        }
        if (*(array->values + array->lpos) !=0 ) {
            printf("\"%s\" ],", *(array->names + array->lpos));
        } else {
            printf("null ],");
        }
        // for bloc
        printf("\n    \"bloc\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            int **xy = *(array->bloc + i);
            if (*(array->values + i) !=0 ) {
                printf("[ [%d, %d], [%d, %d] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
            } else {
                printf("null, ");
            }
        }
        if (*(array->values + array->lpos) !=0 ) {
            int **xy = *(array->bloc + array->lpos);
            printf("[ [%d, %d], [%d, %d] ] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
        } else {
            printf("null ],");
        }
        // for loc
        printf("\n    \"loc\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            int **xy = *(array->loc + i);
            if (*(array->values + i) !=0 ) {
                printf("[ [%d, %d], [%d, %d] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
            } else {
                printf("null, ");
            }
        }
        if (*(array->values + array->lpos) !=0 ) {
            int **xy = *(array->loc + array->lpos);
            printf("[ [%d, %d], [%d, %d] ] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
        } else {
            printf("null ],");
        }

        printf("\n    \"size\" : %d,", array->size);
        printf("\n    \"len\" : %d,", array->len);
        printf("\n    \"apos\" : %d,", array->apos);
        printf("\n    \"lpos\" : %d,", array->lpos);
        printf("\n    \"uval\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            printf("%d, ", *(array->uval + i) );
        } 
        // int val = *(array->u + array->lpos) == NULL ? 0 : 1;
        printf("%d ],", *(array->uval + array->lpos));
        printf("\n    \"cval\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            printf("%d, ", *(array->cval + i) );
        } 
        // int val = *(array->u + array->lpos) == NULL ? 0 : 1;
        printf("%d ],", *(array->cval + array->lpos));
        printf("\n    \"values\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            printf("%d, ", *(array->values + i) );
        } 
        printf("%d ]\n}\n", *(array->values + array->lpos));
    } else {
        printf("\n{\n    \"names\" : null,");
        printf("\n    \"bloc\" : null, ");
        printf("\n    \"loc\" : null, ");
        printf("\n    \"size\" : %d,", array->size);
        printf("\n    \"len\" : %d,", array->len);
        printf("\n    \"apos\" : %d,", array->apos);
        printf("\n    \"lpos\" : %d,", array->lpos);
        printf("\n    \"uval\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            printf("%d, ", *(array->uval + i) );
        } 
        printf("%d ],", *(array->uval + array->lpos) );
        printf("\n    \"cval\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            printf("%d, ", *(array->cval + i) );
        } 
        printf("%d ],", *(array->cval + array->lpos) );
        printf("\n    \"values\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            printf("%d, ", *(array->values + i) );
        } 
        printf("%d ]\n}\n", *(array->values + array->lpos) );
    }
    return;
}

int darray_json_len(darray *array) {
    int len = 0;
    if (array->len > 0) {
        len = len + strlen("\n{\n    \"names\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            if (*(array->values + i) !=0 ) {
                len = len + strlen("\"\", ") + strlen( *(array->names + i) );
            } else {
                len = len + strlen("null, ");
            }
        }
        if (*(array->values + array->lpos) !=0 ) {
            len = len + strlen("\"\" ],") + strlen( *(array->names + array->lpos) );
        } else {
            len = len + strlen("null ],");
        }
        // for bloc
        len = len + strlen("\n    \"bloc\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            int **xy = *(array->bloc + i);
            if (*(array->values + i) !=0 ) {
                len = len + strlen("[ [, ], [, ] ], ") + 4 * 6;
            } else {
                len = len + strlen("null, ");
            }
        }
        if (*(array->values + array->lpos) !=0 ) {
            int **xy = *(array->bloc + array->lpos);
            len = len + strlen("[ [, ], [, ] ] ], ") + 4 * 6;
        } else {
            len = len + strlen("null ], ");
        }
        // for loc
        len = len + strlen("\n    \"loc\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            int **xy = *(array->loc + i);
            if (*(array->values + i) !=0 ) {
                len = len + strlen("[ [, ], [, ] ], ") + 4 * 6;
            } else {
                len = len + strlen("null, ");
            }
        }
        if (*(array->values + array->lpos) !=0 ) {
            int **xy = *(array->loc + array->lpos);
            len = len + strlen("[ [, ], [, ] ] ], ") + 4 * 6;
        } else {
            len = len + strlen("null ], ");
        }

        len = len + strlen("\n    \"size\" : ,") + 6;
        len = len + strlen("\n    \"len\" : ,") + 6;
        len = len + strlen("\n    \"apos\" : ,") + 6;
        len = len + strlen("\n    \"lpos\" : ,") + 6;
        len = len + strlen("\n    \"uval\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            len = len + strlen(", ") + 6;
        } 
        len = len + strlen(" ],") + 6;
        len = len + strlen("\n    \"cval\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            len = len + strlen(", ") + 6;
        } 
        len = len + strlen(" ],") + 6;
        len = len + strlen("\n    \"values\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            len = len + strlen(", ") + 6;
        } 
        len = len + strlen(" ]\n}\n") + 6;
    } else {
        len = len + strlen("\n{\n    \"names\" : null,");
        len = len + strlen("\n    \"bloc\" : null, ");
        len = len + strlen("\n    \"loc\" : null, ");
        len = len + strlen("\n    \"size\" : ,") + 6;
        len = len + strlen("\n    \"len\" : ,") + 6;
        len = len + strlen("\n    \"apos\" : ,") + 6;
        len = len + strlen("\n    \"lpos\" : ,") + 6;
        len = len + strlen("\n    \"uval\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            len = len + strlen(", ") + 6;
        } 
        len = len + strlen(" ],") + 6;
        len = len + strlen("\n    \"cval\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            len = len + strlen(", ") + 6;
        } 
        len = len + strlen(" ],") + 6;
        len = len + strlen("\n    \"values\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            len = len + strlen(", ") + 6;
        } 
        len = len + strlen(" ]\n}\n") + 6;
    }
    return len;
}

char* darray_json_string(darray *array) {
    // return allocated string pointer
    int len = darray_json_len(array); int t = 0;
    char *str = malloc(len + 1);
    if (str == NULL) {
        printf("\njson string cannot be allocated!!\n");
        return NULL;
    }
    if (array->len > 0) {
        t += sprintf(str + t, "\n{\n    \"names\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            if (*(array->values + i) !=0 ) {
                t += sprintf(str + t,"\"%s\", ", *(array->names + i));
            } else {
                t += sprintf(str + t, "null, ");
            }
        }
        if (*(array->values + array->lpos) !=0 ) {
            t += sprintf(str + t, "\"%s\" ],", *(array->names + array->lpos));
        } else {
            t += sprintf(str + t, "null ],");
        }

        // for bloc
        t += sprintf(str + t, "\n    \"bloc\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            int **xy = *(array->bloc + i);
            if (*(array->values + i) !=0 ) {
                t += sprintf(str + t, "[ [%d, %d], [%d, %d] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
            } else {
                t += sprintf(str + t, "null, ");
            }
        }
        if (*(array->values + array->lpos) !=0 ) {
            int **xy = *(array->bloc + array->lpos);
            t += sprintf(str + t, "[ [%d, %d], [%d, %d] ] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
        } else {
            t += sprintf(str + t, "null ],");
        }
        // for loc
        t += sprintf(str + t, "\n    \"loc\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            int **xy = *(array->loc + i);
            if (*(array->values + i) !=0 ) {
                t += sprintf(str + t, "[ [%d, %d], [%d, %d] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
            } else {
                t += sprintf(str + t, "null, ");
            }
        }
        if (*(array->values + array->lpos) !=0 ) {
            int **xy = *(array->loc + array->lpos);
            t += sprintf(str + t, "[ [%d, %d], [%d, %d] ] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
        } else {
            t += sprintf(str + t, "null ],");
        }

        t += sprintf(str + t, "\n    \"size\" : %d,", array->size);
        t += sprintf(str + t, "\n    \"len\" : %d,", array->len);
        t += sprintf(str + t, "\n    \"apos\" : %d,", array->apos);
        t += sprintf(str + t, "\n    \"lpos\" : %d,", array->lpos);
        t += sprintf(str + t, "\n    \"uval\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            t += sprintf(str + t, "%d, ", *(array->uval + i) );
        } 
        t += sprintf(str + t, "%d ],", *(array->uval + array->lpos));
        t += sprintf(str + t, "\n    \"cval\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            t += sprintf(str + t, "%d, ", *(array->cval + i) );
        } 
        t += sprintf(str + t, "%d ],", *(array->cval + array->lpos));
        t += sprintf(str + t, "\n    \"values\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            t += sprintf(str + t, "%d, ", *(array->values + i) );
        } 
        t += sprintf(str + t, "%d ]\n}\n", *(array->values + array->lpos));
    } else {
        t += sprintf(str + t, "\n{\n    \"names\" : null,");
        t += sprintf(str + t, "\n    \"bloc\" : null, ");
        t += sprintf(str + t, "\n    \"loc\" : null, ");
        t += sprintf(str + t, "\n    \"size\" : %d,", array->size);
        t += sprintf(str + t, "\n    \"len\" : %d,", array->len);
        t += sprintf(str + t, "\n    \"apos\" : %d,", array->apos);
        t += sprintf(str + t, "\n    \"lpos\" : %d,", array->lpos);
        t += sprintf(str + t, "\n    \"uval\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            t += sprintf(str + t, "%d, ", *(array->uval + i) );
        } 
        t += sprintf(str + t, "%d ],", *(array->uval + array->lpos) );
        t += sprintf(str + t, "\n    \"cval\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            t += sprintf(str + t, "%d, ", *(array->cval + i) );
        } 
        t += sprintf(str + t, "%d ],", *(array->cval + array->lpos) );
        t += sprintf(str + t, "\n    \"values\" : [ ");
        for (int i = 0; i < array->lpos; i++) {
            t += sprintf(str + t, "%d, ", *(array->values + i) );
        } 
        t += sprintf(str + t, "%d ]\n}\n", *(array->values + array->lpos) );
    }
    *(str + t) = '\0'; // added null character at last
    return str;
}

int darray_json_file(darray *array) {
    // write json string into file tout.json
    char *s = darray_json_string(array);
    FILE *fp = fopen("tout.json", "w");
    if (fp == NULL) {
        printf("\ntout.json file cannot be opened in written mode!!\n");
        return 0;
    }
    int r = fprintf(fp, "%s", s);
    if (r < 0) {
        printf("\njson data cannot be written to file!\n");
        return 0;
    }
    fclose(fp);
    int len = strlen(s);
    free(s);
    return len;
}


ucode* gen_ucode() {
    int size = 1;
    int len = 0;
    int ***bloc = malloc(sizeof (int **) * size);
    int ***loc = malloc(sizeof (int **) * size);

    ucode *u = malloc(sizeof (ucode));
    u->bloc = bloc;
    u->loc = loc;
    u->len = len;
    u->size = size;

    return u;
}

void ucode_append(ucode *u, int bloc[2][2], int loc[2][2]) {
    // add element to last continous availible position [apos]
    if (u->len >= u->size) {
        int ***b; int ***l;
        b = realloc(u->bloc, sizeof (int **) * u->size * 2);
        l = realloc(u->loc, sizeof (int **) * u->size * 2);
        if (b == NULL || l == NULL) {
            printf("\nloc values memory cannot be reallocated for append\n");
            return;
        }
        u->bloc = b;
        u->loc = l;
        u->size = u->size * 2;
    }
    int **bxy = gen_xy(bloc);
    int **xy = gen_xy(loc);
    if (bxy == NULL || xy == NULL) {
        printf("\n memory not allocated for xy(ucode_append)\n");
        return;
    }

    *(u->bloc + u->len) = bxy; // added xy cordinate pair(array) in loc
    *(u->loc + u->len) = xy; // added xy cordinate pair(array) in loc
    u->len = u->len + 1;
    return;
}

void ucode_show_json(ucode *uc) {
    // print line by line in json format
    if (uc->len > 0) {
        // for bloc
        printf("\n{");
        printf("\n    \"bloc\" : [ ");
        for (int i = 0; i < uc->len - 1; i++) {
            int **xy = *(uc->bloc + i);
            if ( xy != NULL ) {
                printf("[ [%d, %d], [%d, %d] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
            } else {
                printf("null, ");
            }
        }
        // int ***a = (uc->bloc + uc->len);
        // int **b = *(uc->bloc + uc->len);
        if (*(uc->bloc + uc->len - 1) != NULL ) {
            int **xy = *(uc->bloc + uc->len - 1);
            printf("[ [%d, %d], [%d, %d] ] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
        } else {
            printf("null ],");
        }
        // for loc
        printf("\n    \"loc\" : [ ");
        for (int i = 0; i < uc->len - 1; i++) {
            int **xy = *(uc->loc + i);
            if ( xy != NULL ) {
                printf("[ [%d, %d], [%d, %d] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
            } else {
                printf("null, ");
            }
        }
        if (*(uc->loc + uc->len - 1) != NULL ) {
            int **xy = *(uc->loc + uc->len - 1);
            printf("[ [%d, %d], [%d, %d] ] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
        } else {
            printf("null ],");
        }

        printf("\n    \"len\" : %d,", uc->len);
        printf("\n    \"size\" : %d\n}\n", uc->size);
    }
}

int ucode_json_len(ucode *uc) {
    int len = 0;
    if (uc->len > 0) {
        // for bloc
        len  = len + strlen("\n{");
        len  = len + strlen("\n    \"bloc\" : [ ");
        for (int i = 0; i < uc->len - 1; i++) {
            int **xy = *(uc->bloc + i);
            if ( xy != NULL ) {
                len = len + strlen("[ [, ], [, ] ] ], ") + 4 * 6;
            } else {
                len  = len + strlen("null, ");
            }
        }
        // int ***a = (uc->bloc + uc->len);
        // int **b = *(uc->bloc + uc->len);
        if (*(uc->bloc + uc->len - 1) != NULL ) {
            int **xy = *(uc->bloc + uc->len - 1);
            len = len + strlen("[ [, ], [, ] ] ], ") + 4 * 6;
        } else {
            len  = len + strlen("null ],");
        }
        // for loc
        len  = len + strlen("\n    \"loc\" : [ ");
        for (int i = 0; i < uc->len - 1; i++) {
            int **xy = *(uc->loc + i);
            if ( xy != NULL ) {
                len = len + strlen("[ [, ], [, ] ] ], ") + 4 * 6;
            } else {
                len  = len + strlen("null, ");
            }
        }
        if (*(uc->loc + uc->len - 1) != NULL ) {
            int **xy = *(uc->loc + uc->len - 1);
            len = len + strlen("[ [, ], [, ] ] ], ") + 4 * 6;
        } else {
            len  = len + strlen("null ],");
        }

        len = len + strlen("\n    \"len\" : ,") + 6;
        len = len + strlen("\n    \"size\" : \n}\n") + 6;
    }

    return len;
}


char* ucode_json_string(ucode *uc) {
    // return ucode json string
    int len = ucode_json_len(uc); int t = 0;
    char *str = malloc(len + 1);
    if (uc->len > 0) {
        // for bloc
        t += sprintf(str + t, "\n{");
        t += sprintf(str + t, "\n    \"bloc\" : [ ");
        for (int i = 0; i < uc->len - 1; i++) {
            int **xy = *(uc->bloc + i);
            if ( xy != NULL ) {
                t += sprintf(str + t, "[ [%d, %d], [%d, %d] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
            } else {
                t += sprintf(str + t, "null, ");
            }
        }
        // int ***a = (uc->bloc + uc->len);
        // int **b = *(uc->bloc + uc->len);
        if (*(uc->bloc + uc->len - 1) != NULL ) {
            int **xy = *(uc->bloc + uc->len - 1);
            t += sprintf(str + t, "[ [%d, %d], [%d, %d] ] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
        } else {
            t += sprintf(str + t, "null ],");
        }
        // for loc
        t += sprintf(str + t, "\n    \"loc\" : [ ");
        for (int i = 0; i < uc->len - 1; i++) {
            int **xy = *(uc->loc + i);
            if ( xy != NULL ) {
                t += sprintf(str + t, "[ [%d, %d], [%d, %d] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
            } else {
                t += sprintf(str + t, "null, ");
            }
        }
        if (*(uc->loc + uc->len - 1) != NULL ) {
            int **xy = *(uc->loc + uc->len - 1);
            t += sprintf(str + t, "[ [%d, %d], [%d, %d] ] ], ", *(*( xy + 0) + 0), *(*( xy + 0) + 1), *(*( xy + 1) + 0), *(*( xy + 1) + 1));
        } else {
            t += sprintf(str + t, "null ],");
        }

        t += sprintf(str + t, "\n    \"len\" : %d,", uc->len);
        t += sprintf(str + t, "\n    \"size\" : %d\n}\n", uc->size);
    }
    *(str + t) = '\0'; // added null character at last
    return str;
}

int ucode_json_file(ucode *uc) {
    // write json string into file tout2.json
    char *s = ucode_json_string(uc);
    FILE *fp = fopen("tout2.json", "w");
    if (fp == NULL) {
        printf("\ntout2.json file cannot be opened in written mode!!\n");
        return 0;
    }
    int r = fprintf(fp, "%s", s);
    if (r < 0) {
        printf("\nt2 json data cannot be written to file!\n");
        return 0;
    }
    fclose(fp);
    int len = strlen(s);
    free(s);
    return len;
}

int ud_json_file(darray *array) {
    // write json string into file tout3.json
    char *s = darray_json_string(array);
    FILE *fp = fopen("tout3.json", "w");
    if (fp == NULL) {
        printf("\ntout3.json file cannot be opened in written mode!!\n");
        return 0;
    }
    int r = fprintf(fp, "[\n\n");
    r += fprintf(fp, "%s", s);
    r += fprintf(fp, ",\n");
    if (r < 0) {
        printf("\n3 json data cannot be written to file!\n");
        return 0;
    }
    for (int i = 0; i < array->lpos + 1; i++) {
        if ( *(array->uval + i) == 1 ) {
            ucode *uc = *(array->u + i);
            char *t = ucode_json_string(uc);
            r += fprintf(fp, "\n");
            r += fprintf(fp, "%s", t);
            r += fprintf(fp, ",\n");
            free(t);
        }
    }
    r += fprintf(fp, "\n{}\n");
    r += fprintf(fp, "\n]\n");
    fclose(fp);
    free(s);
    return r;
}

void ucode_free(ucode *u) {
    // delete the darray
    for (int i = 0; i < u->len; i++) {
        // freeing xy blocks bloc & loc
        free(*(*(u->bloc + i) + 0));  // free {x1,y1}
        free(*(*(u->bloc + i) + 1));
        free(*(u->bloc + i));  // // free location of element

        free(*(*(u->loc + i) + 0));  // free {x1,y1}
        free(*(*(u->loc + i) + 1));
        free(*(u->loc + i));  // // free location of element
    }
    free(u->bloc);
    free(u->loc);
    free(u);
    return;
}

/* not so optimized function */

int** gen_xy(int loc[2][2]) {
    // allocate memory for loc array
    int **xy = malloc(sizeof (int *) * 2);
    if (xy == NULL) {
        printf("\n memory not allocated for xy(gen_xy)\n");
        return NULL;
    }
    *(xy + 0) = malloc(sizeof (int) * 2);
    *(xy + 1) = malloc(sizeof (int) * 2);
    if (*(xy + 0) == NULL || *(xy + 1) == NULL) {
        printf("\n memory not allocated for xy values\n");
        return NULL;
    }
    *(*(xy + 0) + 0) = loc[0][0]; // add x1
    *(*(xy + 0) + 1) = loc[0][1]; // add y1
    *(*(xy + 1) + 0) = loc[1][0]; // add x2
    *(*(xy + 1) + 1) = loc[1][1]; // add y2

    return xy;
}

void get_xy(darray *array,int pos, int loc[2][2]) {
    // save the values from darry to loc
    loc[0][0] = *(*(*(array->loc + pos) + 0) + 0);  // getting x1 cordinate from loc
    loc[0][1] = *(*(*(array->loc + pos) + 0) + 1);  // getting y1 
    loc[1][0] = *(*(*(array->loc + pos) + 1) + 0);  // getting x2 
    loc[1][1] = *(*(*(array->loc + pos) + 1) + 1);  // getting y2
}

void set_xy(darray *array,int pos, int loc[2][2]) {
    // set the values from loc to darray
    *(*(*(array->loc + pos) + 0) + 0) = loc[0][0];  // replacing x1 cordinate from loc
    *(*(*(array->loc + pos) + 0) + 1) = loc[0][1];  
    *(*(*(array->loc + pos) + 1) + 0) = loc[1][0];  
    *(*(*(array->loc + pos) + 1) + 1) = loc[1][1]; 
}

void get_bxy(darray *array,int pos, int bloc[2][2]) {
    // save the values from darry to loc
    bloc[0][0] = *(*(*(array->bloc + pos) + 0) + 0);  // getting x1 cordinate from loc
    bloc[0][1] = *(*(*(array->bloc + pos) + 0) + 1);  // getting y1 
    bloc[1][0] = *(*(*(array->bloc + pos) + 1) + 0);  // getting x2 
    bloc[1][1] = *(*(*(array->bloc + pos) + 1) + 1);  // getting y2
}

void set_bxy(darray *array,int pos, int bloc[2][2]) {
    // set the values from loc to darray
    *(*(*(array->bloc + pos) + 0) + 0) = bloc[0][0];  // replacing x1 cordinate from loc
    *(*(*(array->bloc + pos) + 0) + 1) = bloc[0][1];  
    *(*(*(array->bloc + pos) + 1) + 0) = bloc[1][0];  
    *(*(*(array->bloc + pos) + 1) + 1) = bloc[1][1]; 
}


void uget_bxy(ucode *uc,int pos, int bloc[2][2]) {
    // save the values from darry to loc
    bloc[0][0] = *(*(*(uc->bloc + pos) + 0) + 0);  // getting x1 cordinate from loc
    bloc[0][1] = *(*(*(uc->bloc + pos) + 0) + 1);  // getting y1 
    bloc[1][0] = *(*(*(uc->bloc + pos) + 1) + 0);  // getting x2 
    bloc[1][1] = *(*(*(uc->bloc + pos) + 1) + 1);  // getting y2
}

void uset_bxy(ucode *uc,int pos, int bloc[2][2]) {
    // set the values from loc to darray
    *(*(*(uc->bloc + pos) + 0) + 0) = bloc[0][0];  // replacing x1 cordinate from loc
    *(*(*(uc->bloc + pos) + 0) + 1) = bloc[0][1];  
    *(*(*(uc->bloc + pos) + 1) + 0) = bloc[1][0];  
    *(*(*(uc->bloc + pos) + 1) + 1) = bloc[1][1]; 
}

void uget_xy(ucode *uc,int pos, int loc[2][2]) {
    // save the values from darry to loc
    loc[0][0] = *(*(*(uc->loc + pos) + 0) + 0);  // getting x1 cordinate from loc
    loc[0][1] = *(*(*(uc->loc + pos) + 0) + 1);  // getting y1 
    loc[1][0] = *(*(*(uc->loc + pos) + 1) + 0);  // getting x2 
    loc[1][1] = *(*(*(uc->loc + pos) + 1) + 1);  // getting y2
}

void uset_xy(ucode *uc,int pos, int loc[2][2]) {
    // set the values from loc to darray
    *(*(*(uc->loc + pos) + 0) + 0) = loc[0][0];  // replacing x1 cordinate from loc
    *(*(*(uc->loc + pos) + 0) + 1) = loc[0][1];  
    *(*(*(uc->loc + pos) + 1) + 0) = loc[1][0];  
    *(*(*(uc->loc + pos) + 1) + 1) = loc[1][1]; 
}
