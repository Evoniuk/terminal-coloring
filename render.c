#include "common.h"

char* color_sequence(char letter)
{
    return
        letter == 'l' ? ESCAPE "40m":
        letter == 'r' ? ESCAPE "41m":
        letter == 'g' ? ESCAPE "42m":
        letter == 'y' ? ESCAPE "43m":
        letter == 'b' ? ESCAPE "44m":
        letter == 'm' ? ESCAPE "45m":
        letter == 'c' ? ESCAPE "46m":
        letter == 'w' ? ESCAPE "47m":
        letter == 'L' ? ESCAPE "100m":
        letter == 'R' ? ESCAPE "101m":
        letter == 'G' ? ESCAPE "102m":
        letter == 'Y' ? ESCAPE "103m":
        letter == 'B' ? ESCAPE "104m":
        letter == 'M' ? ESCAPE "105m":
        letter == 'C' ? ESCAPE "106m":
        letter == 'W' ? ESCAPE "107m":
                        ESCAPE "49m";
}

void draw_row(int row)
{
    for (int col = 0; col < E.num_cols; col++)
    {
        printf("%s", color_sequence(E.state[row][col]));

        if (row == E.cursor_row && col == E.cursor_col)   // if cursor, draw it
        {
            if (E.state[row][col] == 'w' || E.state[row][col] == 'W')
                printf(ESCAPE "5;31m_" ESCAPE "25;39m"); // red cursor on white background
            else printf(ESCAPE "5m_" ESCAPE "25m");
        }

        else printf(" ");                                // spaces for the background to color
    }

    printf("\r\n");                                       // go to the first column on the next line
}

// EXTERNAL:

void draw_screen()
{
    printf(ESCAPE "H"); // start at (0, 0)
    for (int row = 0; row < E.num_rows; row++)
        draw_row(row);
}
