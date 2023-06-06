// Structures and function declarations that have something to do with the game itself

#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>

#include "terminal_f.h"
#include "iList.h"
#include "util.h"

// Data structures

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
    int highScore;
    float snakeSpeed;
    iList_t snakeBody;
    char moveDir;
    char lastMoved;

    int appleX;
    int appleY;
    short eaten;

    float accel;
    float startAccel;
    float maxAccel;
    int startLength;
    int startSpeed;

} game_t;

// Game Update functions

void game_updateMoveDir(game_t * data);

void game_updateSnake(game_t * data);

void game_updateCollisions(game_t * data);

void game_updateApple(game_t * data);

// Game Draw functions

void game_drawSnake(game_t data);

void game_drawApple(int appleX, int appleY);

// Game Other functions

// Resets only the snake body and speed
void game_resetSnake(game_t * data);

// Resets all game stats for gameplay, keeps general stuff
void game_resetGame(game_t * data);

// Resets everything in the game_t structure
void game_reset(game_t * data);

void game_genApple(game_t * data);

#endif