#include "common.h"

void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios); // restore original settings
    printf(ESCAPE "2J" ESCAPE "H" ESCAPE "?25h");        // clear screen, go home, cursor visible
}

void enable_raw_mode()
{
    printf(ESCAPE "2J" ESCAPE "?25l");                  // clear screen, cursor invisible

    tcgetattr(STDIN_FILENO, &E.orig_termios);           // write original config to orig_termios
    struct termios raw = E.orig_termios;                // create a new termios for raw mode

    raw.c_iflag &= ~(BRKINT | ICRNL  | INPCK  | ISTRIP | IXON);
    raw.c_lflag &= ~(ECHO   | ICANON | IEXTEN | ISIG);
    raw.c_oflag &= ~(OPOST);                            // disable output processing that converts \n to \r\n
    raw.c_cflag |=  (CS8);                              // set character size to 8 bits per byte (just in case)

    raw.c_cc[VMIN]  = 1; // cc = control characters. VMIN sets number of bytes of input needed to be read before
    raw.c_cc[VTIME] = 1; // `read` can return. VTIME sets time to wait before read returns in tenths of a second

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);           // set raw mode settings
    atexit(disable_raw_mode);                           // disable raw mode when program ends
}
