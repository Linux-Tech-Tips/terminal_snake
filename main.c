#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>

#include <stdbool.h>
#include <signal.h>
#include <time.h>

#include "terminal_f.h"
#include "iList.h"
#include "game.h"

// === Compile-time constants ===

#define UPS 120

// === Signal handling ===

static volatile bool run = true;
static int pSigID;

void signalHandle(int sigID) {
	pSigID = sigID;
	run = false;
}

// === General utility functions ===

// floating point (rational) time in seconds
double timeToSec(struct timespec t) {
	return (double) (t.tv_sec + (t.tv_nsec/1000000000.0));
}

/* 
	TODO:
		- Important utility functions:
			- iList_t one specific function
				- check if pair of ints (each pair from 0 further, basically 0+n;1+n, for natural n) exists in array
			- File functions
				- save data to file
				- load data from file
*/


// === Main executive functions ===

void update(game_t * data) {
	// Reading last character  input
	short res = nbRead(&data->lastKey, 1);
	if(!res)
		data->lastKey = 0;
	if(data->lastKey == 'q') run = false;

	switch(data->lastKey) {
		case 'w':
			if(data->lastMoved != DOWN) // Preventing moving into itself
				data->moveDir = UP;
			break;
		case 's':
			if(data->lastMoved != UP)
				data->moveDir = DOWN;
			break;
		case 'a':
			if(data->lastMoved != RIGHT)
				data->moveDir = LEFT;
			break;
		case 'd':
			if(data->lastMoved != LEFT)
				data->moveDir = RIGHT;
			break;
	}

	// WORK IN PROGRESS - SUBJECT TO CHANGE
	if(data->moveTimer <= 0) {
		// Removing first coord pair
		iList_del(&data->snakeBody, 0);
		iList_del(&data->snakeBody, 0);
		// Getting last coord pair
		int cX;
		int cY;
		iList_get(data->snakeBody, iList_len(data->snakeBody)-2, &cX);
		iList_get(data->snakeBody, iList_len(data->snakeBody)-1, &cY);
		switch(data->moveDir) {
			case UP:
				cY -= 1;
				break;
			case DOWN:
				cY += 1;
				break;
			case LEFT:
				cX -= 1;
				break;
			case RIGHT:
				cX += 1;
				break;
		}
		// Adding new coord pair
		iList_push(&data->snakeBody, cX);
		iList_push(&data->snakeBody, cY);
		data->lastMoved = data->moveDir;
		// Resetting move timer
		data->moveTimer = 1.0f/data->snakeSpeed;
	} else {
		data->moveTimer -= data->delta;
	}

	/* 
		TODO:
			- Features:
				- pause/play
				- game state (over/paused/playing), changing between game states
				- snake collision checking
				- apple generation, snake apple collision checking, growing in length if apple eaten
				- score (eaten apples) counting + saving
				- snake speed getting faster based on score
	*/

}

void render(game_t * data) {

	// === DRAWING SNAKE ===
	modeReset();
	// Erasing last part
	int cX;
	int cY;
	iList_get(data->snakeBody, 0, &cX);
	iList_get(data->snakeBody, 1, &cY);
	cursorMoveTo(cX, cY);
	printf(" ");
	// Drawing tail
	modeSet(STYLE_BOLD, FG_GREEN, BG_DEFAULT);
	iList_get(data->snakeBody, 2, &cX);
	iList_get(data->snakeBody, 3, &cY);
	cursorMoveTo(cX, cY);
	printf("o");
	// Drawing snake body
	for(int i = 4; i < (iList_len(data->snakeBody)-2); i += 2) {
		iList_get(data->snakeBody, i, &cX);
		iList_get(data->snakeBody, (i+1), &cY);
		cursorMoveTo(cX, cY);
		printf("O");
	}
	// Drawing snake head
	iList_get(data->snakeBody, iList_len(data->snakeBody)-2, &cX);
	iList_get(data->snakeBody, iList_len(data->snakeBody)-1, &cY);
	cursorMoveTo(cX, cY);
	switch(data->moveDir) {
		case UP:
			printf("v");
			break;
		case DOWN:
			printf("^");
			break;
		case LEFT:
			printf(">");
			break;
		case RIGHT:
			printf("<");
			break;
	}

	// === DRAWING GUI ===
	modeReset();
	cursorHome();
	// Drawing walls
	modeSet(NO_CODE, FG_DEFAULT, BG_WHITE);
	for(int i = 0; i <= data->termX; i++) {
		cursorMoveTo(i, 0);
		printf(" ");
		cursorMoveTo(i, data->termY);
		printf(" ");
	}
	for(int i = 0; i < data->termY; i++) {
		cursorMoveTo(0, i);
		printf(" ");
		cursorMoveTo(data->termX, i);
		printf(" ");
	}
	// Drawing FPS
	cursorHome();
	modeSet(NO_CODE, FG_BLACK, BG_WHITE);
	float fps = (float) (1.0 / data->delta);
	printf("FPS: %.2f", fps);

	/*
		TODO:
			- GUI
				- different depending on game state
			- Game
				- render walls
				- render apples if anywhere
	*/

	fflush(stdout); // Makes sure everything is actually shown
}

int main() {

	// Capturing SIGINT, to terminate properly even on ctrl+c
	signal(SIGINT, signalHandle);
	fflush(stdout);

	// Terminal GUI init
	screenSave();
	erase();
	cursorHide();
	startKeys();

	// Game data
	game_t gameData = { .frameTime = 1.0f/UPS, .snakeSpeed = 6 };
	gameData.moveTimer = 1.0f/gameData.snakeSpeed;
	gameData.snakeBody = iList_init();
	gameData.moveDir = DOWN; // TODO Decide on starting direction based on available terminal space
	gameData.lastMoved = gameData.moveDir;
	
	// Time variables
	double prevTime = 0; // Time at the start of the latest update
	struct timespec now = {}; // Reused structure for current time at any point necessary
	
	// Additional data init - WIP Move elsewhere, change a bit
	iList_push(&gameData.snakeBody, 10);
	iList_push(&gameData.snakeBody, 10);
	iList_push(&gameData.snakeBody, 10);
	iList_push(&gameData.snakeBody, 11);
	iList_push(&gameData.snakeBody, 10);
	iList_push(&gameData.snakeBody, 12);
	iList_push(&gameData.snakeBody, 10);
	iList_push(&gameData.snakeBody, 13);
	iList_push(&gameData.snakeBody, 10);
	iList_push(&gameData.snakeBody, 14);

	// Main game loop
	while(run) {
		// Get time at the start of update
		clock_gettime(CLOCK_MONOTONIC, &now);
		gameData.delta = timeToSec(now) - prevTime; // Get delta - how long the last frame took
		prevTime = timeToSec(now);

		getTerminalSize(&gameData.termX, &gameData.termY); // Updating terminal size (in case of change)

		// Game logic update
		update(&gameData);

		// Terminal frame render
		render(&gameData);

		// Get time at the end of update
		clock_gettime(CLOCK_MONOTONIC, &now);
		float uTime = timeToSec(now) - prevTime; // Total time taken by update

		// Sleeping for remaining frame time if any frame time remaining (based on defined updates per second - UPS compile-time constant)
		if(uTime < gameData.frameTime) {
			struct timespec st = {};
			long sleepTime = (gameData.frameTime - uTime)*1000000000.0f;
			st.tv_nsec = sleepTime % 1000000000;
			st.tv_sec = (time_t) (sleepTime / 1000000000);
			nanosleep(&st, NULL);
		}
	}

	iList_free(&gameData.snakeBody);

	// Terminal GUI termination
	cursorShow();
	screenRestore();
	endKeys();

    return 0;
}
