#include "common.h"
#include "raw-mode.h"
#include "initialize.h"
#include "render.h"
#include "input.h"

int main(int argc, char** argv)
{
    enable_raw_mode();
    init_editor(argv[1]);

    while (1)
    {
        draw_screen();
        get_input();
    }

    return 0;
}
