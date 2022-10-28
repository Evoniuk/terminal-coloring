#include "common.h"

void save_file()
{
    FILE* file = fopen(E.filename, "w");

    fprintf(file, "%d\n%d\n", E.num_rows, E.num_cols);
    for (int row = 0; row < E.num_rows; row++)
    {
        for (int col = 0; col < E.num_cols; col++)
            fprintf(file, "%c", E.state[row][col]);
        fprintf(file, "\n");
    }

    fclose(file);
}

void get_input()
{
    char c;
    read(STDIN_FILENO, &c, 1);

    switch(c)
    {
        // handle arrow keys
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
            E.state[E.cursor_row][E.cursor_col] = '0';
            break;

        // handle meta stuff
        case 's': // save
            if (E.filename) save_file();
            break;
        case 'q': // quit
            exit(0);
            break;
        default: break;
    }
}
