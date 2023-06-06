// Utility functions file, generally anything utility which should be usable (includable) in other files

#ifndef SNAKE_UTIL_H
#define SNAKE_UTIL_H

#include <time.h>

#include "iList.h"

// Constants
#define UPS 60

// floating point (rational) time in seconds
double util_timeToSec(struct timespec t);

// Takes the given iList as a list of coordinates (x,y), tests whether the given (x,y) pair is in the list
short util_coordsInList(iList_t l, int x, int y);

// Takes only a section of the given list, offset specifies beginning index (or end index if negative)
short util_coordsInList_o(iList_t l, int x, int y, int offset);

// Max integer from a pair
int util_iMax(int a, int b);
// Min integer from a pair
int util_iMin(int a, int b);

// TODO Implement file functions (file loading or saving)

#endif