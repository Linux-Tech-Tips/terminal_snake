#include "util.h"

// Utility functions (for snake)

double util_timeToSec(struct timespec t) {
	return (double) (t.tv_sec + (t.tv_nsec/1000000000.0));
}

short util_coordsInList(iList_t l, int x, int y) {
    short result = 0;
    // Test only if list length odd (list can be interpreted as a list of (x,y) coord pairs)
    if(iList_len(l) % 2 == 0) {
        for(int i = 0; i < iList_len(l); i += 2) {
            int cX, cY;
            iList_get(l, i, &cX);
            iList_get(l, i+1, &cY);
            result = result || (cX == x && cY == y);
        }
    }
    return result;
}
