#ifndef COMMON_H
#define COMMON_H

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ESCAPE "\x1b["
#define NUM_ROWS 32
#define NUM_COLS 80

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
    struct termios orig_termios;    // original terminal attributes
};

struct screen_state E;

#endif
