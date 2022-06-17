#include "../program.c"
#include "renderer.c"

static int win_width = 800;
static int win_height = 600;

typedef struct
{
    GLFWwindow* window;
    mu_Context muContext;
    ProgramData data;
} ProgramMemory;

static ProgramMemory programMemory = {};

static const char KEY_MAP[256] = {
    [GLFW_KEY_LEFT_SHIFT  & 0xff] = MU_KEY_SHIFT,
    [GLFW_KEY_RIGHT_SHIFT  & 0xff] = MU_KEY_SHIFT,
    [GLFW_KEY_LEFT_CONTROL & 0xff] = MU_KEY_CTRL,
    [GLFW_KEY_RIGHT_CONTROL & 0xff] = MU_KEY_CTRL,
    [GLFW_KEY_LEFT_ALT  & 0xff] = MU_KEY_ALT,
    [GLFW_KEY_RIGHT_ALT  & 0xff] = MU_KEY_ALT,
    [GLFW_KEY_ENTER & 0xff] = MU_KEY_RETURN,
    [GLFW_KEY_BACKSPACE  & 0xff] = MU_KEY_BACKSPACE,
};

static void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    int c = KEY_MAP[key & 0xff];
    if (action == GLFW_PRESS)
    {
        printf("%s, key press:%d\n", __FUNCTION__, c);
        mu_input_keydown(&programMemory.muContext, c);
    }
    else if (action == GLFW_RELEASE)
    {
        printf("%s, key release:%d\n", __FUNCTION__, c);
        mu_input_keyup(&programMemory.muContext, c);
    }
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    mu_input_mousemove(&programMemory.muContext, xpos, ypos);
}

static const char BUTTON_MAP[256] = {
    [GLFW_MOUSE_BUTTON_LEFT & 0xff] = MU_MOUSE_LEFT,
    [GLFW_MOUSE_BUTTON_RIGHT & 0xff] = MU_MOUSE_RIGHT,
    [GLFW_MOUSE_BUTTON_MIDDLE & 0xff] = MU_MOUSE_MIDDLE,
};

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    printf("button:%d, action:%d, mods:%d\n", button, action, mods);
    int b = BUTTON_MAP[button & 0xff];
    if (action == GLFW_PRESS)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        printf("%s, mouse press:%d\n", __FUNCTION__, b);
        mu_input_mousedown(&programMemory.muContext, x, y, b);
    }
    else if (action == GLFW_RELEASE)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        printf("%s, mouse release:%d\n", __FUNCTION__, b);
        mu_input_mouseup(&programMemory.muContext, x, y, b);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    mu_input_scroll(&programMemory.muContext, 0, yoffset * -30);
}

void loop_handler()
{
    GLFWwindow* window = programMemory.window;
    mu_Context* muCtx = &programMemory.muContext;

    glfwPollEvents();

    r_getSize(&win_width, &win_height);
    // log_info("width:%d, height:%d", win_width, win_height);

    program_update(&programMemory.data, muCtx, win_width, win_height);

    /* render */
    r_resize(win_width, win_height);
    r_clear(mu_color(100, 100, 100, 255));
    mu_Command* cmd = NULL;
    
    glEnable(GL_SCISSOR_TEST);
    while (mu_next_command(muCtx, &cmd))
    {
        switch (cmd->type)
        {
            case MU_COMMAND_TEXT:
                r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color);
                break;
            case MU_COMMAND_RECT:
                r_draw_rect(cmd->rect.rect, cmd->rect.color);
                break;
            case MU_COMMAND_ICON:
                r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color);
                break;
            case MU_COMMAND_CLIP:
                r_set_clip_rect(cmd->clip.rect);
                break;
        }
    }
    glDisable(GL_SCISSOR_TEST);
    r_present();
}

static int text_width(mu_Font font, const char* text, int len)
{
    if (len == -1)
    {
        len = strlen(text);
    }
    return r_get_text_width(text, len);
}

static int text_height(mu_Font font)
{
    return r_get_text_height();
}

int main(int argc, char* argv[])
{
    r_resize(640, 640);
    programMemory.window = r_init();
    if (!programMemory.window)
    {
        printf("window init failed\n");
        return -1;
    }

    mu_init(&programMemory.muContext);
    programMemory.muContext.text_width = text_width;
    programMemory.muContext.text_height = text_height;

    program_init(&programMemory.data);

    glfwSetKeyCallback(programMemory.window, key_callback);
    glfwSetCursorPosCallback(programMemory.window, cursor_pos_callback);
    glfwSetMouseButtonCallback(programMemory.window, mouse_button_callback);
    glfwSetScrollCallback(programMemory.window, scroll_callback);

    while (!glfwWindowShouldClose(programMemory.window))
    {
        loop_handler();
    }

    return 0;
}
