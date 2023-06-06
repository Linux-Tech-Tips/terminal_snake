#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>

#include <stdbool.h>
#include <signal.h>
#include <time.h>

#include "terminal_f.h"
#include "iList.h"
#include "game.h"
#include "util.h"
#include "gui.h"

// === Compile-time constants ===

#define UPS 60

// === Signal handling ===

static volatile bool run = true;
static int pSigID;

void signalHandle(int sigID) {
	pSigID = sigID;
	run = false;
}

// === Game-important functions ===

void update(game_t * data) {
	// Reading last character  input
	short res = nbRead(&data->lastKey, 1);
	if(!res)
		data->lastKey = 0;
	if(data->lastKey == 'q') run = false;

	if(data->state == running) {
		// Running logic
		if(!(data->paused)) {
			// Updating snake if playing
			game_updateMoveDir(data);
			game_updateSnake(data);
			// TODO Update apples
		} else {
			gui_updatePause(data);
		}

		// Pause toggle
		if(data->lastKey == 'p')
			data->paused = !data->paused;

	} else if(data->state == over) {
		// Game over logic
		gui_updateGameOver(data);
	} else if(data->state == menu) {
		// Menu logic
		gui_updateMenu(data);
	}
}

void render(game_t * data) {

	// Erasing screen if screen changed
	if(data->lastKey == 'p' || data->lastKey == 'm')
		erase();
	// Erasing screen if terminal resized
	if(data->resized)
		erase();

	// Always draw walls + fps
	gui_drawWalls(data->termX, data->termY);
	gui_drawFPS(0, 0, data->delta);

	if(data->state == menu) {
		// TODO Draw menu
		gui_drawMenu(data->termX, data->termY);
	} else {
		// Drawing game if running or over
		game_drawSnake(*data);

		// Drawing pause menu if game paused
		if(data->paused)
			gui_drawPause(data->termX, data->termY);
		if(data->state == over) {
			gui_drawGameOver(data->termX, data->termY);
		}
	}


	// Should be at the end of loop - remainder of screen gets filled with current mode if erased
	modeReset();

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
	game_t gameData = { .frameTime = 1.0f/UPS, .snakeSpeed = 6, .paused = 0, .state = menu };
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
		gameData.delta = util_timeToSec(now) - prevTime; // Get delta - how long the last frame took
		prevTime = util_timeToSec(now);

		// Updating terminal size, checking whether resized
		int lastX = gameData.termX;
		int lastY = gameData.termY;
		getTerminalSize(&gameData.termX, &gameData.termY);
		if(lastX == gameData.termX && lastY == gameData.termY)
			gameData.resized = 0;
		else
			gameData.resized = 1;

		// Game logic update
		update(&gameData);

		// Terminal frame render
		render(&gameData);

		// Get time at the end of update
		clock_gettime(CLOCK_MONOTONIC, &now);
		float uTime = util_timeToSec(now) - prevTime; // Total time taken by update

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
