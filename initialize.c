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

char* read_file(char* filename)
{
    FILE* file = fopen(filename, "r");

    if (!file) return NULL;

    fseek(file, 0L, SEEK_END);
    long numbytes = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char* buffer = calloc(numbytes + 1, sizeof(char));
    fread(buffer, sizeof(char), numbytes, file);

    fclose(file);

    return buffer;
}

int set_state(char* file_contents)
{
    if (sscanf(file_contents, "%d\n", &E.num_rows) == EOF) return -1;
    while (*file_contents++ != '\n'); // skip to next line
    if (sscanf(file_contents, "%d\n", &E.num_cols) == EOF) return -1;
    while (*file_contents++ != '\n');

    for (int row = 0; row < E.num_rows; row++)
    {
        for (int col = 0; col < E.num_cols; col++)
            E.state[row][col] = *file_contents++;
        if (*file_contents++ != '\n') return -1;
    }

    return 0;
}

// EXTERNAL:

void init_editor(char* filename)
{
    E.cursor_row       = 0;
    E.cursor_col       = 0;
    E.editing_filename = 0;
    E.has_been_edited  = 0;
    E.filename         = filename;

    int file_error = 0;
    if (filename)
    {
        char* file_contents = read_file(filename);
        if (file_contents && strlen(file_contents) > 0)
        {
            file_error = set_state(file_contents);
            free(file_contents);
        }
        else file_error = 1;
    }

    if (!filename || file_error)
        memset(E.state, '0', sizeof(E.state)); // zero out state

    get_window_size(&E.screen_rows, &E.screen_cols);
    E.num_rows = NUM_ROWS > E.screen_rows ? E.screen_rows : NUM_ROWS; // This should be checked regardless of file status
    E.num_cols = NUM_COLS > E.screen_cols ? E.screen_cols : NUM_COLS;
}
