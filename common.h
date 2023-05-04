#ifndef COMMON_H
#define COMMON_H

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ESCAPE "\x1b["
#define NUM_ROWS 32
#define NUM_COLS 80

#define BLACK     "40m"
#define RED       "41m"
#define GREEN     "42m"
#define YELLOW    "43m"
#define BLUE      "44m"
#define MAGENTA   "45m"
#define CYAN      "46m"
#define WHITE     "47m"
#define B_BLACK   "100m"
#define B_RED     "101m"
#define B_GREEN   "102m"
#define B_YELLOW  "103m"
#define B_BLUE    "104m"
#define B_MAGENTA "105m"
#define B_CYAN    "106m"
#define B_WHITE   "107m"
#define CLEAR     "49m"

enum editor_key
{
    BACKSPACE  =  127, // ASCII value of backspace
    ARROW_LEFT = 1000, // avoid conflict with 'normal' character presses
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN,
};

struct screen_state
{
    int    screen_rows;
    int    screen_cols;
    int    cursor_row;
    int    cursor_col;
    int    num_rows;
    int    num_cols;
    char   state[NUM_ROWS][NUM_COLS];
    char*  filename;
    char   status_message[80];
    char*  status_message_fg;
    char*  status_message_bg;
    int    editing_filename;
    int    has_been_edited;
    int    drawing_mode;
    struct termios orig_termios;    // original terminal attributes
};

struct screen_state E;

#endif
