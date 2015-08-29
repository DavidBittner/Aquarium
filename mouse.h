#include <GL/glfw3.h>

bool mouseClick = false;
bool mouseHold = false;
int mousex = 0, mousey = 0;

void MousePosFunc( GLFWwindow *wind, double x, double y )
{

    mousex = x; mousey = y;

}

void MouseButtonFunc( GLFWwindow *window, int button, int action, int mods )
{

    if( button == GLFW_MOUSE_BUTTON_LEFT )
    {

        if( action == GLFW_PRESS )
        {

            mouseClick = true;
            mouseHold = true;

        }
        else
        {

            mouseHold = false;

        }

    }

}

void ResetKeys()
{

    if( mouseClick )
        mouseClick = false;

}
