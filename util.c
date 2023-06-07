#include "util.h"

// Utility functions (for snake)

double util_timeToSec(struct timespec t) {
	return (double) (t.tv_sec + (t.tv_nsec/1000000000.0));
}

short util_coordsInList(iList_t l, int x, int y) {
    return util_coordsInList_o(l, x, y, 0);
}

short util_coordsInList_o(iList_t l, int x, int y, int offset) {
    short result = 0;
    // Test only if list length odd (list can be interpreted as a list of (x,y) coord pairs)
    if(iList_len(l) % 2 == 0) {
        for(int i = (offset > 0 ? offset : 0); i < iList_len(l) + (offset < 0 ? offset : 0); i += 2) {
            int cX, cY;
            iList_get(l, i, &cX);
            iList_get(l, i+1, &cY);
            result = result || (cX == x && cY == y);
        }
    }
    return result;
}

int util_iMax(int a, int b) {
    return (a > b ? a : b);
}
int util_iMin(int a, int b) {
    return (a < b ? a : b);
}

void util_saveScore(char * fileName, int highScore) {
    FILE * file;
    file = fopen(fileName, "w");
    fprintf(file, "%i\n", highScore);
    fclose(file);
}

int util_loadScore(char * fileName) {
    // Opening file
    FILE * file;
    file = fopen(fileName, "r");
    if(file == NULL)
        return 0;
    // Reading line
    char * line = NULL;
    size_t n = 0;
    getline(&line, &n, file);
    // Converting to int (with error checking)
    errno = 0;
    long readScore = strtol(line, NULL, 10);
    int score = 0;
    if(errno == 0)
        score = (int) readScore;
    // Freeing remaining allocated things + return
    free(line);
    fclose(file);
    return score;
}
