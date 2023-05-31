#ifndef ESC_FUNC_H
#define ESC_FUNC_H

/* Escape code defines */
#define ESCAPE "\x1b["
#define ESCAPE_PURE "\x1b"
#define NO_CODE -1
// Foreground colors
#define FG_BLACK 30
#define FG_RED 31
#define FG_GREEN 32
#define FG_YELLOW 33
#define FG_BLUE 34
#define FG_MAGENTA 35
#define FG_CYAN 36
#define FG_WHITE 37
#define FG_DEFAULT 39
// Background colors
#define BG_BLACK 40
#define BG_RED 41
#define BG_GREEN 42
#define BG_YELLOW 43
#define BG_BLUE 44
#define BG_MAGENTA 45
#define BG_CYAN 46
#define BG_WHITE 47
#define BG_DEFAULT 49
// Styles
#define STYLE_BOLD 1
#define STYLE_FAINT 2
#define STYLE_ITALIC 3
#define STYLE_UNDERLINE 4
#define STYLE_BLINKING 5
#define STYLE_REVERSE 7
#define STYLE_HIDDEN 8
#define STYLE_STRIKETHROUGH 9
// Directions
#define UP 'A'
#define DOWN 'B'
#define LEFT 'D'
#define RIGHT 'C'
// Line erase options
#define LINE_TO_END 0
#define LINE_FROM_START 1
#define LINE_ALL 2

// TERMINAL FUNCTIONS

/* Resets all modes (styles and colors) */
void modeReset();

/* Sets the mode */
void modeSet(int style, int fgColor, int bgColor);

/* Sets a 256 color mode */
void mode256Color(int bg, int colorCode);

/* Erases the screen */
void erase();

/* Erases on the current line, according to the specified variable */
void eraseLine(int lineType);

/* Move cursor to home position (0,0) */
void cursorHome();

/* Move cursor to specified column and line (x, y) */
void cursorMoveTo(int column, int line);

/* Move cursor by the specified amount in the set dir */
void cursorMoveBy(char dir, int amount);

/* Gets terminal dimensions from the kernel, works even if terminal window is resized */
void getTerminalSize(int * x, int * y);

/* Sets terminal options for unbuffered read */
void startKeys();

/* Non-blocking character read, needs startKeys to be called first */
bool nbRead(char * buffer, size_t maxToRead);

/* Resets terminal options */
void endKeys();

void cursorHide();
void cursorShow();

void screenSave();
void screenRestore();

#endif
