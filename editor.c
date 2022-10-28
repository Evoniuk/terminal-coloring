#include <string.h>
#include <sys/ioctl.h>
#include "common.h"

int get_cursor_position(int* rows, int* cols)
{
    char buffer[32];

    printf(ESCAPE "6n"); // request cursor position. This escape
                         // sequence writes ESC[{row};{col}R to stdin
    unsigned int i;
    for (i = 0; i < sizeof(buffer) - 1; i++)
        if (read(STDIN_FILENO, &buffer[i], 1) != 1 || (buffer[i] == 'R'))
            break; // if read fails or we get to the R we stop

    buffer[i] = '\0'; // null terminate

    if (buffer[0] != '\x1b' || // if what was read doesn't match the
        buffer[1] != '['    || // format we return -1 to indicate failure
        sscanf(&buffer[2], "%d;%d", rows, cols) != 2)
        return -1;

    return 0; // return 0 to indicate success
}

int get_window_size(int* rows, int* cols)
{
    struct winsize ws; // winsize defined in sys/ioctl.h

    // TIOCGWINSZ = Terminal IO Control Get WINdow SiZe
    // Here, we read the window size into the winsize struct
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    {
        // in the case that ioctl fails or ws_col is 0, move the cursor
        // 999 times right and down to reach the bottom right corner
        printf(ESCAPE "999C" ESCAPE "999B");
        return get_cursor_position(rows, cols);
    }

    *rows = ws.ws_row;
    *cols = ws.ws_col;

    return 0;
}

void init_editor(char* filename)
{
    E.cursor_row = 0;
    E.cursor_col = 0;
    E.filename   = filename;

    memset(E.state, 0, sizeof(E.state)); // zero out state
    get_window_size(&E.screen_rows, &E.screen_cols);

    E.screen_cols /= 2; // 'cause each "pixel" is two characters wide

    // set rows and cols to either predefined values or screen size, whichever is smaller
    E.num_rows = NUM_ROWS > E.screen_rows ? E.screen_rows : NUM_ROWS;
    E.num_cols = NUM_COLS > E.screen_cols ? E.screen_cols : NUM_COLS;
}
