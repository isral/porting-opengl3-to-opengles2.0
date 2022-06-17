#define USE_MICROUI
#define _CRT_SECURE_NO_WARNINGS
#include "../microui.h"

#define GLFM_INCLUDE_ES32

#include "extern/glfm/glfm.h"
#include "renderer.c"


#include "../program.c"

typedef struct
{
    mu_Context muContext;
    ProgramData data;
} ProgramMemory;

static void onFrame(GLFMDisplay* display);
static void onSurfaceCreated(GLFMDisplay* display, int width, int height);
static void onSurfaceDestroyed(GLFMDisplay* display);

void glfmMain(GLFMDisplay* display)
{
    glfmSetDisplayConfig(
        display,
        GLFMRenderingAPIOpenGLES2,
        GLFMColorFormatRGBA8888,
        GLFMDepthFormatNone,
        GLFMStencilFormatNone,
        GLFMMultisampleNone);
    glfmSetSurfaceCreatedFunc(display, onSurfaceCreated);
    glfmSetSurfaceResizedFunc(display, onSurfaceCreated);
    glfmSetSurfaceDestroyedFunc(display, onSurfaceDestroyed);
    glfmSetRenderFunc(display, onFrame);
    r_init(display);
}

static void onSurfaceCreated(GLFMDisplay* display, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void onSurfaceDestroyed(GLFMDisplay* display)
{
}

static void onFrame(GLFMDisplay* display)
{
    static ProgramMemory program = {};
    static mu_Context* muCtx = &program.muContext;

    int win_width, win_height;
    glfmGetDisplaySize(display, &win_width, &win_height);
    program_update(&program.data, muCtx, win_width, win_height);

    r_clear(mu_color(100, 100, 100, 255));

    mu_Command* cmd = NULL;
    glEnable(GL_SCISSOR_TEST);
    while (mu_next_command(muCtx, &cmd))
    {
        switch (cmd->type)
        {
            case MU_COMMAND_TEXT:
                r_draw_text(
                    cmd->text.str, cmd->text.pos, cmd->text.color);
                break;
            case MU_COMMAND_RECT:
                r_draw_rect(cmd->rect.rect, cmd->rect.color);
                break;
            case MU_COMMAND_ICON:
                r_draw_icon(
                    cmd->icon.id, cmd->icon.rect, cmd->icon.color);
                break;
            case MU_COMMAND_CLIP:
                r_set_clip_rect(cmd->clip.rect);
                break;
        }
    }
    glDisable(GL_SCISSOR_TEST);

    r_present();
}