#include "common.h"
#include "raw-mode.h"
#include "initialize.h"
#include "render.h"
#include "input.h"

int main(int argc, char** argv)
{
    E.drawing_mode = 1;

    int option;
    while ((option = getopt(argc, argv, ":p:")) != -1)
    {
        switch(option)
        {
            case 'p':
                E.filename = optarg;
                E.drawing_mode = 0;
                break;
            case ':':
                fprintf(stderr, "You must provide a file.\n");
                exit(0);
                break;
        }
    }

    if (!E.drawing_mode)
    {
        init_editor(E.filename);
        draw_rows();
    }

    else
    {
        enable_raw_mode();
        init_editor(argv[1]);

        while (1)
        {
            draw_screen();
            get_input();
        }
    }
    
    return 0;
}
