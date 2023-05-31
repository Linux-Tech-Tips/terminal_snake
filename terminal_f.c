#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h> // UNIX std library - not multiplatform
#include <sys/ioctl.h>
#include <poll.h>
#include <stdbool.h>

#include "terminal_f.h"

// VARIABLES

static struct termios oldTerm;

// TERMINAL FUNCTIONS

/* Resets all modes (styles and colors) */
void modeReset() {
	printf(ESCAPE "0m");
}

/* Sets the mode */
void modeSet(int style, int fgColor, int bgColor) {
	printf(ESCAPE);
	if(style != NO_CODE) printf("%i", style);
	printf(";");
	if(fgColor != NO_CODE) printf("%i", fgColor);
	if(bgColor != NO_CODE) printf(";%i", bgColor);
	printf("m");
}

/* Sets a 256 color mode */
void mode256Color(int bg, int colorCode) {
	int mode = bg ? 48 : 38;
	printf(ESCAPE "%i;5;%im", mode, colorCode);
}

/* Erases the screen */
void erase() {
	printf(ESCAPE "2J");
}

/* Erases on the current line, according to the specified variable */
void eraseLine(int lineType) {
	printf(ESCAPE "%iK", lineType);
}

/* Move cursor to home position (0,0) */
void cursorHome() {
	printf(ESCAPE "H");
}

/* Move cursor to specified column and line (x, y) */
void cursorMoveTo(int column, int line) {
	printf(ESCAPE "%i;%iH", line, column);
}

/* Move cursor by the specified amount in the set dir */
void cursorMoveBy(char dir, int amount) {
	printf(ESCAPE "%i%c", amount, dir);
}

/* Gets terminal dimensions from the kernel, works even if terminal window is resized */
void getTerminalSize(int * x, int * y) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	*x = size.ws_col;
	*y = size.ws_row;
}

/* Sets terminal options for unbuffered read */
void startKeys() {
	struct termios newTerm;
	tcgetattr(STDIN_FILENO, &oldTerm);
	newTerm = oldTerm;
	newTerm.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);
}

/* Non-blocking character read, needs startKeys to be called first, returns whether any characters read */
bool nbRead(char * buffer, size_t maxToRead) {
	bool result = 0;
	struct pollfd fds;
	fds.fd = 1;
	fds.events = POLLIN;
	int ready = poll(&fds, 1, 0);
	char c;
	int r = 0;
	while(ready > 0 && read(STDIN_FILENO, &c, 1) > 0) {
		result = 1;
		if(r < maxToRead)
			buffer[r] = c;
		ready = poll(&fds, 1, 0);
		++r;
	}
	return result;
}

/* Resets terminal options */
void endKeys() {
	tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
}

void cursorHide() {
	printf(ESCAPE "?25l");
}
void cursorShow() {
	printf(ESCAPE "?25h");
}

void screenSave() {
	printf(ESCAPE "?47h");
}
void screenRestore() {
	printf(ESCAPE "?47l");
}
