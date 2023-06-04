// Structures and functions that have something to do with the game itself

#ifndef GAME_H
#define GAME_H

#include "iList.h"

// Game structure:
// last pressed key, delta, frame time, move timer, game state, score, snake speed, snake body, next move direction, last moved direction

typedef enum { menu, playing, paused, over } gameState_t;

typedef struct {

    // Terminal / user interaction

    int termX, termY;
    char lastKey;

    // Time

    double delta;
    double frameTime;
    float moveTimer;

    // Game itself (snake)

    gameState_t state;
    int score;
    int snakeSpeed;
    iList_t snakeBody;
    char moveDir;
    char lastMoved;

} game_t;

#endif