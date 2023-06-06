// Utility functions file, generally anything utility which should be usable (includable) in other files

#ifndef SNAKE_UTIL_H
#define SNAKE_UTIL_H

#include <time.h>

#include "iList.h"

// floating point (rational) time in seconds
double util_timeToSec(struct timespec t);

// Takes the given iList as a list of coordinates (x,y), tests whether the given (x,y) pair is in the list
short util_coordsInList(iList_t l, int x, int y);

// Max integer from a pair
int util_iMax(int a, int b);
// Min integer from a pair
int util_iMin(int a, int b);

// TODO Implement file functions (file loading or saving)

#endif