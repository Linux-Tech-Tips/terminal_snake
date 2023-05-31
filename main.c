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
void update(float delta) {
	// Reading last character  input
	char c = 0; // Array of length 2 so it is null terminated
	short res = nbRead(&c, 1);
	if(c == 'q') run = false;

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

void render(float delta, float frameTime) {
	// Stub calls for now, customize later
	modeReset();
	cursorHome();
	printf("Current delta time: %.6f", delta);
	cursorHome();
	cursorMoveBy(DOWN, 1);
	printf("Total frame time: %f", frameTime);

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

	fflush(stdout);
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
	float delta = 0.0f;
	float frameTime = 1.0f/UPS;
	int termX, termY;

	// Main game loop
	while(run) {
		// Get time at the start of update
		struct timespec now = {};
		clock_gettime(CLOCK_MONOTONIC, &now);

		getTerminalSize(&termX, &termY); // Updating terminal size (in case terminal dynamically changed)

		// Game logic update
		update(delta);

		// Terminal frame render
		render(delta, frameTime);

		// Get time at the end of update
		long lasts = now.tv_sec;
		long lastns = now.tv_nsec;
		clock_gettime(CLOCK_MONOTONIC, &now);
		delta = (float) ((now.tv_sec + (now.tv_nsec/1000000000.0)) - (lasts + (lastns/1000000000.0)));

		// Sleeping for remaining frame time if any frame time remaining
		if(delta < frameTime) {
			struct timespec st = {};
			st.tv_nsec = (frameTime - delta)*1000000000.0f;
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
