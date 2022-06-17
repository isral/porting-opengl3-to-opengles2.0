#include "microui.c"
#include "program.h"

#include <stdio.h>

void program_update(ProgramData* data, mu_Context* muCtx, int win_width, int win_height)
{
    mu_begin(muCtx);

    if (mu_begin_window(muCtx, "My Window", mu_rect(0, 0, 800, 600)))
    {
        mu_layout_row(muCtx, 2, (int[]){60, -1}, 0);

        mu_label(muCtx, "First:");
        if (mu_button(muCtx, "Button1"))
        {
            printf("Button1 pressed\n");
        }

        mu_label(muCtx, "Second:");
        if (mu_button(muCtx, "Button2"))
        {
            mu_open_popup(muCtx, "My Popup");
        }

        if (mu_begin_popup(muCtx, "My Popup"))
        {
            mu_label(muCtx, "Hello world!");
            mu_end_popup(muCtx);
        }

        mu_end_window(muCtx);
    }

    mu_end(muCtx);
}

void program_init(ProgramData* data)
{

}