#include "common.h"
#include "render.h"

#define CTRL_KEY(c) ((c) & 0x1f)

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

int read_key_press()
{
    char c;
    read(STDIN_FILENO, &c, 1);

    if (c == '\x1b')
    {
        char seq[3]; // if ESC, read in sequence
        if (read(STDIN_FILENO, &seq[0], 1) != 1)
            return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1)
            return '\x1b';
        if (seq[0] == '[')
        {
            if (seq[1] >= '0' && seq[1] <= '9')
            {
                if (read(STDIN_FILENO, &seq[2], 1) != 1)
                    return '\x1b';
                if (seq[2] == '~')
                {
                    switch (seq[1])
                    {
                        case '1': return HOME_KEY;
                        case '3': return DEL_KEY;
                        case '4': return END_KEY;
                        case '5': return PAGE_UP;
                        case '6': return PAGE_DOWN;
                        case '7': return HOME_KEY;
                        case '8': return END_KEY;
                    }
                }
            }
            else
            {
                switch (seq[1])
                {
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                    case 'H': return HOME_KEY;
                    case 'F': return END_KEY;
                }
            }
        }
        else if (seq[0] == 'O')
        {
            switch (seq[1])
            {
                case 'H': return HOME_KEY;
                case 'F': return END_KEY;
            }
        }
        return '\x1b';
    }
    else return c;
}

void create_file() // TODO: handle the creation of a file
{
    #define MAX_FILENAME_LEN 64
    E.filename = calloc(MAX_FILENAME_LEN, sizeof(char));
    E.editing_filename = 1;
    draw_screen(); // to render leading '>'

    char c = read_key_press();
    for (int i = 0; (c != '\r') && (i < MAX_FILENAME_LEN - 1); c = read_key_press())
    {
        if (c == BACKSPACE) E.filename[--i] = '\0';
        else E.filename[i++] = c;
        draw_screen();
    }

    E.editing_filename = 0;
    save_file();
}

// EXTERNAL:

void get_input()
{
    int c = read_key_press();

    switch(c)
    {
        // handle arrow keys
        case ARROW_UP:
            if (E.cursor_row > 0) E.cursor_row--;
            break;
        case ARROW_DOWN:
            if (E.cursor_row < E.num_rows - 1) E.cursor_row++;
            break;
        case ARROW_LEFT:
            if (E.cursor_col > 0) E.cursor_col--;
            break;
        case ARROW_RIGHT:
            if (E.cursor_col < E.num_cols - 1) E.cursor_col++;
            break;

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
            if (E.state[E.cursor_row][E.cursor_col] != c)
            {
                E.has_been_edited = 1;
                E.state[E.cursor_row][E.cursor_col] = c;
            }
            break;
        case BACKSPACE: // erase
            if (E.state[E.cursor_row][E.cursor_col] != '0')
            {
                E.has_been_edited = 1;
                E.state[E.cursor_row][E.cursor_col] = '0';
            }
            break;

        // handle meta stuff
        case CTRL_KEY('s'): // save
            E.has_been_edited = 0;
            if (E.filename) save_file();
            else create_file();
            break;
        case CTRL_KEY('q'): // quit
            exit(0);
            break;
        default: break;
    }
}
