#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>

#include <stdbool.h>
#include <signal.h>
#include <time.h>

#include "terminal_f.h"


// === Compile-time constants ===
#define UPS 60;

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
	float delta;
	float frameTime;
	
	// Timer data
	float moveTimer;
};

enum gameState { menu, playing, paused, over };

struct game {
	// General game data
	enum gameState state;
	int score;

	// Snake-specific data
	int snakeSpeed;
	int snakeBody [2];
	char moveDir;
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
/* 
	TODO:
		- Important utility functions:
			- Array functions
				- allocate array
				- resize array
				- push array (push to front, shift everything else by one spot to the right)
				- pop array (pop from a position, collapse the rest)
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
			gd->moveDir = UP;
			break;
		case 's':
			gd->moveDir = DOWN;
			break;
		case 'a':
			gd->moveDir = LEFT;
			break;
		case 'd':
			gd->moveDir = RIGHT;
			break;
	}

	// WORK IN PROGRESS - SUBJECT TO CHANGE
	if(md->moveTimer <= 0) {
		switch(gd->moveDir) {
			case UP:
				gd->snakeBody[1] -= 1;
				break;
			case DOWN:
				gd->snakeBody[1] += 1;
				break;
			case LEFT:
				gd->snakeBody[0] -= 1;
				break;
			case RIGHT:
				gd->snakeBody[0] += 1;
				break;
		}
		md->moveTimer = 1.0f/gd->snakeSpeed;
	} else {
		md->moveTimer -=  md->frameTime;
	}

	/* 
		TODO:
			- Features:
				- pause/play
				- game state (over/paused/playing), changing between game states
				- WASD changes snake dir
				- snake goes in direction with a given speed
				- snake collision checking
				- apple generation, snake apple collision checking, growing in length if apple eaten
				- score (eaten apples) counting + saving
				- snake speed getting faster based on score
	*/

}

void render(struct misc * md, struct game * gd) {
	// WORK IN PROGRESS - SUBJECT TO CHANGE
	modeReset();
	cursorHome();
	printf("Current delta time: %.6f", md->delta);
	cursorHome();
	cursorMoveBy(DOWN, 1);
	printf("Total frame time: %f", md->frameTime);
	cursorHome();
	cursorMoveBy(DOWN, 2);
	printf("Sleep for: %f; move timer: %f", (md->frameTime-md->delta), md->moveTimer);

	cursorHome();
	cursorMoveBy(DOWN, 3);
	if(md->lastKey)
		printf("Last read character: %c", md->lastKey, gd->snakeBody[0]);
	else
		printf("Last read character:  ");

	cursorHome();
	modeSet(STYLE_BOLD, FG_BLUE, BG_DEFAULT);
	cursorMoveTo(gd->snakeBody[0], gd->snakeBody[1]);
	printf("O");

	cursorHome();
	modeReset();
	cursorMoveTo(gd->snakeBody[0]+1, gd->snakeBody[1]);
	printf(" ");
	cursorMoveTo(gd->snakeBody[0]-1, gd->snakeBody[1]);
	printf(" ");
	cursorMoveTo(gd->snakeBody[0], gd->snakeBody[1]+1);
	printf(" ");
	cursorMoveTo(gd->snakeBody[0], gd->snakeBody[1]-1);
	printf(" ");

	/*
		TODO:
			- GUI
				- different depending on game state
				- always in top left - FPS made from delta and frametime
			- Game
				- render walls
				- render snake (make it look somehow nice, colorful or styled maybe? + head depending on rotation > ^ < v)
				- render apples if anywhere
	*/

	fflush(stdout); // Makes sure each frame is actually shown
					// Without this, some frames are skipped - possibly WIP add as option for less resource usage?
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
	struct misc miscData = {};
	struct game gameData = {};
	
	// Specific data init
	miscData.frameTime = 1.0f/UPS;
	gameData.snakeSpeed = 1;
	miscData.moveTimer = 1.0f/gameData.snakeSpeed;
	gameData.snakeBody[0] = 10;
	gameData.snakeBody[1] = 10;
	gameData.moveDir = DOWN;

	// Main game loop
	while(run) {
		// Get time at the start of update
		struct timespec now = {};
		clock_gettime(CLOCK_MONOTONIC, &now);

		getTerminalSize(&miscData.termX, &miscData.termY); // Updating terminal size (in case terminal dynamically changed)

		// Game logic update
		update(&miscData, &gameData);

		// Terminal frame render
		render(&miscData, &gameData);

		// Get time at the end of update
		long lasts = now.tv_sec;
		long lastns = now.tv_nsec;
		clock_gettime(CLOCK_MONOTONIC, &now);
		miscData.delta = (float) ((now.tv_sec + (now.tv_nsec/1000000000.0)) - (lasts + (lastns/1000000000.0)));

		// Sleeping for remaining frame time if any frame time remaining
		if(miscData.delta < miscData.frameTime) {
			struct timespec st = {};
			st.tv_nsec = (miscData.frameTime - miscData.delta)*1000000000.0f;
			st.tv_sec = 0;
			nanosleep(&st, NULL);
		}
	}

	// Terminal GUI termination
	cursorShow();
	screenRestore();
	endKeys();

    return 0;
}
