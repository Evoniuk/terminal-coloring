#include "common.h"

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

char* color_sequence(char letter)
{
    return
        letter == 'l' ? ESCAPE BLACK:
        letter == 'r' ? ESCAPE RED:
        letter == 'g' ? ESCAPE GREEN:
        letter == 'y' ? ESCAPE YELLOW:
        letter == 'b' ? ESCAPE BLUE:
        letter == 'm' ? ESCAPE MAGENTA:
        letter == 'c' ? ESCAPE CYAN:
        letter == 'w' ? ESCAPE WHITE:
        letter == 'L' ? ESCAPE B_BLACK:
        letter == 'R' ? ESCAPE B_RED:
        letter == 'G' ? ESCAPE B_GREEN:
        letter == 'Y' ? ESCAPE B_YELLOW:
        letter == 'B' ? ESCAPE B_BLUE:
        letter == 'M' ? ESCAPE B_MAGENTA:
        letter == 'C' ? ESCAPE B_CYAN:
        letter == 'W' ? ESCAPE B_WHITE:
                        ESCAPE CLEAR;
}

void draw_row(int row)
{
    for (int col = 0; col < E.num_cols; col++)
    {
        printf("%s", color_sequence(E.state[row][col]));

        if (row == E.cursor_row && col == E.cursor_col)  // if cursor, draw it
        {
            if (E.state[row][col] == 'w' || E.state[row][col] == 'W')
                printf(ESCAPE "5;31m_" ESCAPE "25;39m"); // red cursor on white background
            else printf(ESCAPE "5m_" ESCAPE "25m");
        }

        else printf(" ");                                // space for the background to color
    }

    printf("\r\n");                                      // go to the first column on the next line
}

void draw_sidebar()
{
    char* sidebar[] = {
        ESCAPE "1;7m"        " COLORS             " ESCAPE "0m",
        ESCAPE RED     "  " ESCAPE "0m" " r        " ESCAPE B_RED     "  " ESCAPE "0m" " R ",
        ESCAPE GREEN   "  " ESCAPE "0m" " g        " ESCAPE B_GREEN   "  " ESCAPE "0m" " G ",
        ESCAPE BLUE    "  " ESCAPE "0m" " b        " ESCAPE B_BLUE    "  " ESCAPE "0m" " B ",
        ESCAPE YELLOW  "  " ESCAPE "0m" " y        " ESCAPE B_YELLOW  "  " ESCAPE "0m" " Y ",
        ESCAPE MAGENTA "  " ESCAPE "0m" " m        " ESCAPE B_MAGENTA "  " ESCAPE "0m" " M ",
        ESCAPE CYAN    "  " ESCAPE "0m" " c        " ESCAPE B_CYAN    "  " ESCAPE "0m" " C ",
        ESCAPE WHITE   "  " ESCAPE "0m" " w        " ESCAPE B_WHITE   "  " ESCAPE "0m" " W ",
        ESCAPE BLACK   "  " ESCAPE "0m" " l        " ESCAPE B_BLACK   "  " ESCAPE "0m" " L ",
        "",
        ESCAPE "1;7m" " COMMANDS           " ESCAPE "0m",
        "CTRL+S          save",
        "CTRL+Q          quit",
        "delete         erase",
        ESCAPE "1;7m" " FILENAME           " ESCAPE "0m",
        E.filename,
        NULL,
    };

    for (int row = 1; sidebar[row]; row++) // row starts at 1 due to 1-indexing
    {                                      // `cols + 2` for padding
        printf(ESCAPE "%d;%dH", row, E.num_cols + 2); // go to appropriate row and column
        printf("%s", sidebar[row]);
    }
}

// EXTERNAL:

void draw_screen()
{
    if (E.num_cols + 22 < E.screen_cols) draw_sidebar(); // 20 character width + 2 padding

    printf(ESCAPE "H"); // start at (0, 0)
    for (int row = 0; row < E.num_rows; row++)
        draw_row(row);
}
