#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
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

void draw_screen();

///////////////////// RAW MODE HANDLING

void enable_raw_mode()
{
    printf(ESCAPE "2J");                                // clear screen
    tcgetattr(STDIN_FILENO, &E.orig_termios);           // write original config to orig_termios
    struct termios raw = E.orig_termios;                // create a new termios for raw mode

    raw.c_iflag &= ~(BRKINT | ICRNL  | INPCK  | ISTRIP | IXON);
    raw.c_lflag &= ~(ECHO   | ICANON | IEXTEN | ISIG);
    raw.c_oflag &= ~(OPOST);                            // disable output processing that converts \n to \r\n
    raw.c_cflag |=  (CS8);                              // set character size to 8 bits per byte (just in case)

    // cc = control characters. VMIN sets number of bytes of input needed to be read before
    // 'read' can return. VTIME sets time to wait before read returns in tenths of a second
    raw.c_cc[VMIN]  = 1;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);           // set the raw mode settings

    printf(ESCAPE "?25l");                              // cursor invisible
}

void disable_raw_mode()
{
    // remove the blinking cursor
    E.cursor_row = -1;
    E.cursor_col = -1;
    draw_screen();

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios);
    printf(ESCAPE "?25h"); // cursor visible
}

///////////////////// EDITOR

int get_cursor_position(int* rows, int* cols)
{
    char buffer[32];

    write(STDOUT_FILENO, ESCAPE "6n", 4); // request cursor position. This escape
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
        write(STDOUT_FILENO, ESCAPE "999C" ESCAPE "999B", 12);
        return get_cursor_position(rows, cols);
    }

    *rows = ws.ws_row;
    *cols = ws.ws_col;

    return 0;
}

void init_editor()
{
    E.cursor_row = 0;
    E.cursor_col = 0;
    E.filename   = NULL;

    memset(E.state, 0, sizeof(E.state)); // zero out state
    get_window_size(&E.screen_rows, &E.screen_cols);

    E.screen_cols /= 2; // 'cause each "pixel" is two characters wide

    // set rows and cols to either predefined values or screen size, whichever is smaller
    E.num_rows = NUM_ROWS > E.screen_rows ? E.screen_rows : NUM_ROWS;
    E.num_cols = NUM_COLS > E.screen_cols ? E.screen_cols : NUM_COLS;
}

///////////////////// INPUT

void get_input()
{
    char c;
    read(STDIN_FILENO, &c, 1);

    switch(c)
    {
        case '\x1b': {
            char seq[2]; // if ESC, read in sequence
            read(STDIN_FILENO, &seq[0], 1);
            read(STDIN_FILENO, &seq[1], 1);

            if (seq[0] == '[')
            {
                if      (seq[1] == 'A' && E.cursor_row > 0) E.cursor_row--;
                else if (seq[1] == 'D' && E.cursor_col > 0) E.cursor_col--;
                else if (seq[1] == 'B' && E.cursor_row < E.num_rows - 1) E.cursor_row++;
                else if (seq[1] == 'C' && E.cursor_col < E.num_cols - 1) E.cursor_col++;
            }

            break;
        }

        // handle colors
        case 'l':
        case 'r':
        case 'g':
        case 'y':
        case 'b':
        case 'm':
        case 'c':
        case 'w':
        case 'L':
        case 'R':
        case 'G':
        case 'Y':
        case 'B':
        case 'M':
        case 'C':
        case 'W':
            E.state[E.cursor_row][E.cursor_col] = c;
            break;
        case 'x': // erase color
            E.state[E.cursor_row][E.cursor_col] = 0;
            break;
        case 'q': // quit
            exit(0);
            break;
        default: break;
    }
}

///////////////////// RENDER

void draw_screen()
{
    int initial_row = E.cursor_row;
    int initial_col = E.cursor_col;

    // print starting at (0, 0)
    printf("%s", ESCAPE "H");

    for (int row = 0; row < E.num_rows; row++)
    {
        for (int col = 0; col < E.num_cols; col++)
        {
            printf("%s", // set background color
                E.state[row][col] == 'l' ? ESCAPE "40m":
                E.state[row][col] == 'r' ? ESCAPE "41m":
                E.state[row][col] == 'g' ? ESCAPE "42m":
                E.state[row][col] == 'y' ? ESCAPE "43m":
                E.state[row][col] == 'b' ? ESCAPE "44m":
                E.state[row][col] == 'm' ? ESCAPE "45m":
                E.state[row][col] == 'c' ? ESCAPE "46m":
                E.state[row][col] == 'w' ? ESCAPE "47m":
                E.state[row][col] == 'L' ? ESCAPE "100m":
                E.state[row][col] == 'R' ? ESCAPE "101m":
                E.state[row][col] == 'G' ? ESCAPE "102m":
                E.state[row][col] == 'Y' ? ESCAPE "103m":
                E.state[row][col] == 'B' ? ESCAPE "104m":
                E.state[row][col] == 'M' ? ESCAPE "105m":
                E.state[row][col] == 'C' ? ESCAPE "106m":
                E.state[row][col] == 'W' ? ESCAPE "107m":
                                           ESCAPE "49m"
            );

            if (row == E.cursor_row && col == E.cursor_col)               // if cursor, draw it
            {
                if (E.state[row][col] == 'w' || E.state[row][col] == 'W') // red cursor on white background
                    printf("%s", ESCAPE "5;31m__" ESCAPE "25;39m");
                else printf("%s", ESCAPE "5m__" ESCAPE "25m");
            }
            else printf("  ");                                             // spaces for the background to color
        }
        printf("\r\n");                                                   // go to the first column on the next line
    }
}

int main(int argc, char** argv)
{
    enable_raw_mode();
    atexit(disable_raw_mode);

    init_editor();
    E.filename = argv[1] ? argv[1] : NULL;

    while (1)
    {
        draw_screen();
        get_input();
    }

    return 0;
}
