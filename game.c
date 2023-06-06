#include "game.h"

// Game Update functions

void game_updateMoveDir(game_t * data) {
    	// Moving depending on last pressed key, 
        //  + checking that the new direction isn't the opposite of the one the snake moved to previously
        //    (prevents the snake from moving into itself)
        switch(data->lastKey) {
		case 'w':
			if(data->lastMoved != DOWN)
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
}


void game_updateSnake(game_t * data) {

    // Executes if timer runs out
    if(data->moveTimer <= 0) {

		// Removing first coord pair
		iList_del(&data->snakeBody, 0);
		iList_del(&data->snakeBody, 0);
		
        // Getting last coord pair
		int cX;
		int cY;
		iList_get(data->snakeBody, iList_len(data->snakeBody)-2, &cX);
		iList_get(data->snakeBody, iList_len(data->snakeBody)-1, &cY);
        
        // Adding new coord pair depending on the direction to move
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
		iList_push(&data->snakeBody, cX);
		iList_push(&data->snakeBody, cY);
        // Saving the direction in which the snake moved (protection against snake turning into itself)
		data->lastMoved = data->moveDir;
		
        // Resetting move timer
		data->moveTimer = 1.0f/data->snakeSpeed;
	} else {        
        // Subtracting timer if necessary
		data->moveTimer -= data->delta;
	}
}

void game_updateCollisions(game_t * data) {
    // Snake head position
    int headX, headY;
    iList_get(data->snakeBody, iList_len(data->snakeBody)-2, &headX);
    iList_get(data->snakeBody, iList_len(data->snakeBody)-1, &headY);

    // Wall collisions
    if((headX < 2 || headX > (data->termX-1)) || (headY < 2 || headY > (data->termY-1))) {
        data->state = over;
    }

    // Snake-to-snake collisions
    if(util_coordsInList_o(data->snakeBody, headX, headY, -2)) {
        data->state = over;
    }
}

// Game Render functions

void game_drawSnake(game_t data) {

    // Style + color reset
    modeReset();

    // Current x and current y of the snake tile being processed
	int cX, cY;
    
	// Erasing snake tail's previous position
	iList_get(data.snakeBody, 0, &cX);
	iList_get(data.snakeBody, 1, &cY);
	cursorMoveTo(cX, cY);
	printf(" ");

	// Drawing tail
	modeSet(STYLE_BOLD, FG_GREEN, BG_DEFAULT); // Style + color for the snake itself
	iList_get(data.snakeBody, 2, &cX);
	iList_get(data.snakeBody, 3, &cY);
	cursorMoveTo(cX, cY);
	printf("o");
	
    // Drawing snake body
	for(int i = 4; i < (iList_len(data.snakeBody)-2); i += 2) {
		iList_get(data.snakeBody, i, &cX);
		iList_get(data.snakeBody, (i+1), &cY);
		cursorMoveTo(cX, cY);
		printf("O");
	}

	// Drawing snake head
	iList_get(data.snakeBody, iList_len(data.snakeBody)-2, &cX);
	iList_get(data.snakeBody, iList_len(data.snakeBody)-1, &cY);
	cursorMoveTo(cX, cY);
	switch(data.moveDir) {
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
}

// Game Other functions

void game_resetSnake(game_t * data) {
    // Resetting array
    iList_free(&data->snakeBody);
    data->snakeBody = iList_init();
    // Setting default snake body + direction
    int defX = util_iMax((int) (0.1f * data->termX), 3);
    int defY  = util_iMax((int) (0.1f * data->termY), 3);
    int len = (data->startLength > 3 && data->startLength < 10) ? data->startLength : 4;
    if(data->termX > data->termY) {
        // Horizontal snake
        data->moveDir = RIGHT;
        for(int i = 0; i < len; i++) {
            iList_push(&data->snakeBody, defX+i);
            iList_push(&data->snakeBody, defY);
        }
    } else {
        // Vertical snake
        data->moveDir = DOWN;
        for(int i = 0; i < len; i++) {
            iList_push(&data->snakeBody, defX);
            iList_push(&data->snakeBody, defY+i);
        }
    }
    data->lastMoved = data->moveDir;
    
    // Resetting other variables
    data->snakeSpeed = (data->startSpeed > 0 && data->startSpeed < 1000) ? data->startSpeed : 6;
    data->moveTimer = 1.0f/data->snakeSpeed;
}

void game_resetGame(game_t * data) {
    // Resetting other game variables
    data->state = menu;
    data->paused = 0;
    data->score = 0;

    data->startLength = 4;
    data->startSpeed = 6;

    // Resetting snake
    game_resetSnake(data);
}

void game_reset(game_t * data) {
    // Resetting other variables
    data->frameTime = 1.0f/UPS;
    data->delta = 0;
    data->lastKey = 0;
    data->resized = 0;
    getTerminalSize(&data->termX, &data->termY);

    // Resetting game
    game_resetGame(data);
}


// TODO Add
//  - apple generation (+ collision checking, regeneration), eating, score + snake speedup
//  - score saving + loading
