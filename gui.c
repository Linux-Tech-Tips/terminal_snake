#include "gui.h"

// INFO FOR GUI: Minimal recommended terminal size is 40*10, TODO Checks


// GUI Update functions

void gui_updatePause(game_t * data) {
    if(data->lastKey == 'm')
		data->state = menu;
}

void gui_updateMenu(game_t * data) {
    if(data->lastKey == 'p') {
        data->state = running;
        data->paused = 0;
    }
}

void gui_updateGameOver(game_t * data) {
    if(data->highScore != data->score) {
        data->highScore = data->score;
    }
    
    if(data->lastKey == 'm') {
        game_resetGame(data);
    } else if(data->lastKey == 'p') {
        game_resetGame(data);
        data->state = running;
    }
}

// GUI Draw functions

void gui_drawWalls(int termX, int termY) {
    // Setting terminal mode
    modeReset();
	cursorHome();
	modeSet(NO_CODE, FG_DEFAULT, BG_WHITE);
    // Drawing horizontal walls
	for(int i = 0; i <= termX; i++) {
		cursorMoveTo(i, 0);
		printf(" ");
		cursorMoveTo(i, termY);
		printf(" ");
	}
    // Drawing vertical walls
	for(int i = 0; i < termY; i++) {
		cursorMoveTo(0, i);
		printf(" ");
		cursorMoveTo(termX, i);
		printf(" ");
	}
}

void gui_drawFPS(int x, int y, float delta) {
    cursorMoveTo(x, y);
	modeSet(NO_CODE, FG_BLACK, BG_WHITE);
	float fps = (float) (1.0 / delta);
	printf("FPS: %.2f", fps);
}

void gui_drawPause(int termX, int termY) {
    int x = util_iMax((int) (0.1f * termX), 3);
    int y = util_iMax((int) (0.1f * termY), 3);
    modeSet(NO_CODE, FG_DEFAULT, BG_DEFAULT);
    cursorMoveTo(x, y);
    printf("GAME PAUSED");
    cursorMoveTo(x, y+1);
    printf(" press 'p' to resume the game");
    cursorMoveTo(x, y+2);
    printf(" press 'm' return to the menu");
    cursorMoveTo(x, y+3);
    printf(" press 'q' to quit the game");
}

void gui_drawMenu(int termX, int termY) {
    int x = util_iMax((int) (termX/2.0f), 3);
    int y = util_iMax((int) (0.15f*termY), 3);
    // Drawing full or minimal menu based on terminal dimensions
    modeSet(STYLE_BOLD, FG_CYAN, BG_DEFAULT);
    if(termX > 55 && termY > 18) {
        // Logo
        cursorMoveTo(x-24, y);
        printf("                              _|                  ");
        cursorMoveTo(x-24, y+1);
        printf("  _|_|_|  _|_|_|      _|_|_|  _|  _|      _|_|    ");
        cursorMoveTo(x-24, y+2);
        printf("_|_|      _|    _|  _|    _|  _|_|      _|_|_|_|  ");
        cursorMoveTo(x-24, y+3);
        printf("    _|_|  _|    _|  _|    _|  _|  _|    _|        ");
        cursorMoveTo(x-24, y+4);
        printf("_|_|_|    _|    _|    _|_|_|  _|    _|    _|_|_|  ");
        // Supporting information
        cursorMoveTo(x-12, y+6);
        printf("Terminal snake game menu");
        cursorMoveTo(x-10, y+7);
        printf(" - Press 'p' to play");
        cursorMoveTo(x-10, y+8);
        printf(" - Press 'q' to exit");
        // Controls information (detailed or minimal depending on available space)
        cursorMoveTo(x-14, y+10);
        printf("Terminal snake game controls");
        if(termX > 60) {
            cursorMoveTo(x-28, y+11);
            printf(" - Press 'W'/'A'/'S'/'D' to change the snake's direction");
            cursorMoveTo(x-28, y+12);
            printf(" - In game, press 'p' to pause or resume the game");
        } else {
            cursorMoveTo(x-17, y+11);
            printf(" - Snake direction: 'W'/'A'/'S'/'D'");
            cursorMoveTo(x-17, y+12);
            printf(" - Pause/resume the game: 'p'");
        }
    } else {
        // Logo
        cursorMoveTo(x-3, y);
        printf("SNAKE");
        // Supporting information
        cursorMoveTo(x-12, y+2);
        printf("Terminal snake game menu");
        cursorMoveTo(x-10, y+3);
        printf(" - Press 'p' to play");
        cursorMoveTo(x-10, y+4);
        printf(" - Press 'q' to exit");
        // Controls information (if space available)
        if(termX > 39 && termY > 13) {
            cursorMoveTo(x-14, y+6);
            printf("Terminal snake game controls");
            cursorMoveTo(x-17, y+7);
            printf(" - Snake direction: 'W'/'A'/'S'/'D'");
            cursorMoveTo(x-17, y+8);
            printf(" - Pause/resume the game: 'p'");
        }
    }
}

void gui_drawGameOver(int termX, int termY, int hitX, int hitY) {
    // TODO Finish proper game over dialog
    int x = util_iMax((int) (0.1f * termX), 3);
    int y = util_iMax((int) (0.1f * termY), 3);
    modeSet(STYLE_BOLD, FG_RED, BG_DEFAULT);
    cursorMoveTo(x, y);
    printf("GAME OVER :(");
    cursorMoveTo(x, y+1);
    printf(" - press 'p' to play again");
    cursorMoveTo(x, y+2);
    printf(" - press 'm' to go to the main menu");
    cursorMoveTo(x, y+3);
    printf(" - press 'q' to quit the game");

    // Rendering red where snake hit (if specified)
    if(hitX >= 0 && hitY >= 0) {
        modeSet(NO_CODE, FG_DEFAULT, BG_RED);
        cursorMoveTo(hitX, hitY);
        printf(" ");
    }
}

// GUI Other functions

short gui_checkTerm(int minX, int minY) {
    int termX, termY;
    getTerminalSize(&termX, &termY);

    if(termX < minX || termY < minY) {
        printf("Current terminal dimensions (%ix%i) smaller than minimum recommended (%ix%i). Continue? (Y/N) ", termX, termY, minX, minY);
        // Reading next stdin character
        char r = getc(stdin);
        return (r == 'Y' || r == 'y');
    } else
        return 1;
}
