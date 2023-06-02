#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>

#include <stdbool.h>
#include <signal.h>
#include <time.h>

#include "terminal_f.h"
#include "iList.h"


// === Compile-time constants ===

#define UPS 120

// === Signal handling ===

static volatile bool run = true;
static int pSigID;

void signalHandle(int sigID) {
	pSigID = sigID;
	run = false;
}

// === Utility data structures ===

struct misc {
	// General terminal data
	int termX, termY;

	// Other general data
	char lastKey;
	double delta;
	double frameTime;
	
	// Timer data
	float moveTimer;
};

typedef enum { menu, playing, paused, over } gameState;

struct game {
	// General game data
	gameState state;
	int score;

	// Snake-specific data
	int snakeSpeed;
	iList snakeBody;
	char moveDir;
	char lastMoved;
};


/* 
	TODO:
		- Data structures containing things:
			- Generic
				- Contains general things
				- current terminal size (X, Y)
				- last pressed key
				- delta time
				- frame time
				- etc.
			- Game
				- Contains game-related (snake-related) things
				- playing/paused/over game state
				- dynamic array of body positions
				- score
				- etc.
*/

// === Utility functions ===

// floating point (rational) time in seconds
double timeToSec(struct timespec t) {
	return (double) (t.tv_sec + (t.tv_nsec/1000000000.0));
}

/* 
	TODO:
		- Important utility functions:
			- iList one specific function
				- check if pair of ints (each pair from 0 further, basically 0+n;1+n, for natural n) exists in array
			- File functions
				- save data to file
				- load data from file
*/


// === Main executive functions ===

void update(struct misc * md, struct game * gd) {
	// Reading last character  input
	short res = nbRead(&md->lastKey, 1);
	if(!res)
		md->lastKey = 0;
	if(md->lastKey == 'q') run = false;

	switch(md->lastKey) {
		case 'w':
			if(gd->lastMoved != DOWN) // Preventing moving into itself
				gd->moveDir = UP;
			break;
		case 's':
			if(gd->lastMoved != UP)
				gd->moveDir = DOWN;
			break;
		case 'a':
			if(gd->lastMoved != RIGHT)
				gd->moveDir = LEFT;
			break;
		case 'd':
			if(gd->lastMoved != LEFT)
				gd->moveDir = RIGHT;
			break;
	}

	// WORK IN PROGRESS - SUBJECT TO CHANGE
	if(md->moveTimer <= 0) {
		// Removing first coord pair
		iList_del(&gd->snakeBody, 0);
		iList_del(&gd->snakeBody, 0);
		// Getting last coord pair
		int cX;
		int cY;
		iList_get(gd->snakeBody, iList_len(gd->snakeBody)-2, &cX);
		iList_get(gd->snakeBody, iList_len(gd->snakeBody)-1, &cY);
		switch(gd->moveDir) {
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
		iList_push(&gd->snakeBody, cX);
		iList_push(&gd->snakeBody, cY);
		gd->lastMoved = gd->moveDir;
		// Resetting move timer
		md->moveTimer = 1.0f/gd->snakeSpeed;
	} else {
		md->moveTimer -= md->delta;
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

void render(struct misc * md, struct game * gd) {

	// === DRAWING SNAKE ===
	modeReset();
	// Erasing last part
	int cX;
	int cY;
	iList_get(gd->snakeBody, 0, &cX);
	iList_get(gd->snakeBody, 1, &cY);
	cursorMoveTo(cX, cY);
	printf(" ");
	// Drawing tail
	modeSet(STYLE_BOLD, FG_GREEN, BG_DEFAULT);
	iList_get(gd->snakeBody, 2, &cX);
	iList_get(gd->snakeBody, 3, &cY);
	cursorMoveTo(cX, cY);
	printf("o");
	// Drawing snake body
	for(int i = 4; i < (iList_len(gd->snakeBody)-2); i += 2) {
		iList_get(gd->snakeBody, i, &cX);
		iList_get(gd->snakeBody, (i+1), &cY);
		cursorMoveTo(cX, cY);
		printf("O");
	}
	// Drawing snake head
	iList_get(gd->snakeBody, iList_len(gd->snakeBody)-2, &cX);
	iList_get(gd->snakeBody, iList_len(gd->snakeBody)-1, &cY);
	cursorMoveTo(cX, cY);
	switch(gd->moveDir) {
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
	for(int i = 0; i <= md->termX; i++) {
		cursorMoveTo(i, 0);
		printf(" ");
		cursorMoveTo(i, md->termY);
		printf(" ");
	}
	for(int i = 0; i < md->termY; i++) {
		cursorMoveTo(0, i);
		printf(" ");
		cursorMoveTo(md->termX, i);
		printf(" ");
	}
	// Drawing FPS
	cursorHome();
	modeSet(NO_CODE, FG_BLACK, BG_WHITE);
	float fps = (float) (1.0 / md->delta);
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

	// Game variables outside of loop
	struct misc miscData = { .frameTime = 1.0f/UPS };
	struct game gameData = { .snakeSpeed = 6, .moveDir = DOWN, .lastMoved = DOWN };
	
	// Time variables
	double prevTime = 0; // Time at the start of the latest update
	struct timespec now = {}; // Reused structure for current time at any point necessary
	
	// Additional data init
	miscData.moveTimer = 1.0f/gameData.snakeSpeed;
	gameData.snakeBody = iList_init();
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
		miscData.delta = timeToSec(now) - prevTime; // Get delta - how long the last frame took
		prevTime = timeToSec(now);

		getTerminalSize(&miscData.termX, &miscData.termY); // Updating terminal size (in case of change)

		// Game logic update
		update(&miscData, &gameData);

		// Terminal frame render
		render(&miscData, &gameData);

		// Get time at the end of update
		clock_gettime(CLOCK_MONOTONIC, &now);
		float uTime = timeToSec(now) - prevTime; // Total time taken by update

		// Sleeping for remaining frame time if any frame time remaining (based on defined updates per second - UPS compile-time constant)
		if(uTime < miscData.frameTime) {
			struct timespec st = {};
			long sleepTime = (miscData.frameTime - uTime)*1000000000.0f;
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
