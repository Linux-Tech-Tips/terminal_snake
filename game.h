// Structures and function declarations that have something to do with the game itself

#ifndef GAME_H
#define GAME_H

#include <stdio.h>

#include "iList.h"
#include "terminal_f.h"

// Game structure:
// last pressed key, delta, frame time, move timer, game state, score, snake speed, snake body, next move direction, last moved direction

typedef enum { menu, running, over } gameState_t;

typedef struct {

    // Terminal / user interaction

    int termX, termY;
    short resized;
    char lastKey;

    // Time

    double delta;
    double frameTime;
    float moveTimer;

    // Game itself (snake)

    gameState_t state;
    short paused;
    int score;
    int snakeSpeed;
    iList_t snakeBody;
    char moveDir;
    char lastMoved;

} game_t;

void game_drawSnake(game_t data);

void game_updateMoveDir(game_t * data);

void game_updateSnake(game_t * data);

#endif