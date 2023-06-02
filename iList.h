// Dynamic (integer) list structure and functions
//  FYI Function name scheme: iList_funcName[_funcMod]()
//      - each separate field separated by underscores is camelCase
//      - the start represents the module (iList for all functions here)
//      - funcMod optionally represents modifier - functions with the same general purpose but slightly different args and thus features

#ifndef I_LIST_H
#define I_LIST_H

#define DEFAULT_SIZE 10 // Default size of allocated buffer
#define ALLOC_CONST 10 // Small constant to add to the increase of the allocated buffer when reached

// Dynamic list structure
typedef struct {
    int occ;
    int total;
    int * arr;
} iList;

iList iList_init();

// Allocate a dynamic list with preset values
iList iList_init_val(int * values, int length);

// Allocate a dynamic list with a specific starting size
iList iList_init_s(int startSize);

void iList_free(iList * l);

// Adds element to the end of the list
void iList_push(iList * l, int element);

void iList_push_s(iList * l, int element, int expandSize);

// Saves element at pos into val if element exists at position, returns success value
short iList_get(iList l, int pos, int * val);

// Deletes element at pos and shrinks rest of list, returns success value
short iList_del(iList * l, int pos);

// Returns amount of items saved in the list
int iList_len(iList l);

#endif