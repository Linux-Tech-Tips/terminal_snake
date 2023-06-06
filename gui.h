// Functions responsible for anything related to GUI

#ifndef SNAKE_GUI_H
#define SNAKE_GUI_H

#include "terminal_f.h"
#include "util.h"
#include "game.h"

// Update functions for GUI

// Pause menu update, executes if in pause menu, can do any desired pause functionality
void gui_updatePause(game_t * data);

// Main menu update, executes in main menu, can do any desired main menu functionality
void gui_updateMenu(game_t * data);

// Game over menu update, executes if game is over, can do any desired game over functionality
void gui_updateGameOver(game_t * data);

// Draw functions for GUI

void gui_drawWalls(int termX, int termY);

void gui_drawFPS(int x, int y, float delta);

void gui_drawPause(int termX, int termY);

void gui_drawMenu(int termX, int termY);

// If non-negative hitX and hitY are specified, a red square is rendered on the position
void gui_drawGameOver(int termX, int termY, int hitX, int hitY);

#endif