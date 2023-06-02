// Dynamic (integer) list structure and functions

#include <stdlib.h>

#include "iList.h"

// List functions

iList iList_init() {
    return iList_init_s(DEFAULT_SIZE);
}

iList iList_init_val(int * values, int length) {
    iList l = iList_init_s(length + DEFAULT_SIZE);
    for(int i = 0; i < length; i++) {
        iList_push(&l, values[i]);
    }
    return l;
}

iList iList_init_s(int startSize) {
    iList l = {};
    l.occ = 0;
    l.total = startSize;
    l.arr = malloc(startSize * sizeof(int));
    return l;
}

void iList_free(iList * l) {
    free(l->arr);
}

void iList_push(iList * l, int element) {
    iList_push_s(l, element, (1.5 * l->total) + ALLOC_CONST);
}

void iList_push_s(iList * l, int element, int expandSize) {
    // Expanding if necessary
    if((l->occ) >= l->total) {
        l->total += expandSize;
        l->arr = realloc(l->arr, l->total * sizeof(int));
    }
    // Adding element to the end
    l->arr[l->occ] = element;
    l->occ++;
}

short iList_get(iList l, int pos, int * val) {
    if(pos < l.occ) {
        *val = l.arr[pos];
        return 1;
    }
    else
        return 0;
}

short iList_del(iList * l, int pos) {
    // Checking if pos valid field to delete
    if(pos >= l->occ)
        return 0;
    // Deleting field, shifting array
    if(pos < (l->occ - 1)) {
        for(int i = pos; i < (l->occ - 1); i++) {
            l->arr[i] = l->arr[i+1];
        }
        l->arr[(l->occ - 1)] = 0;
    } else {
        l->arr[pos] = 0;
    }
    l->occ--;
    return 1;
}

int iList_len(iList l) {
    return l.occ;
}

