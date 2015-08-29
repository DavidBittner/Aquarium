#include <GL/glfw3.h>
#include "ai_fish.h"
#include "mouse.h"

#include <chrono>
#include <thread>

std::vector<FISH> fish;

void Reshape( GLFWwindow *wind, int width, int height )
{

    glViewport( 0, 0, (GLsizei)width, (GLsizei)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, width, 0, height, 1.0f, 10.0f );
    glMatrixMode( GL_MODELVIEW );

}

int main()
{

    glfwInit();
    GLFWwindow *window = glfwCreateWindow( 800, 600, "Test", nullptr, nullptr );
    glfwMakeContextCurrent( window );
    glfwSetWindowSizeCallback( window, Reshape );

    glfwSetCursorPosCallback( window, MousePosFunc );
    glfwSetMouseButtonCallback( window, MouseButtonFunc );

    Reshape( window, 800, 600 );

    glfwGetWindowSize( window, AQUAR_SIZEX, AQUAR_SIZEY );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    while( !glfwWindowShouldClose( window ) )
    {

        glClear( GL_COLOR_BUFFER_BIT );
        glLoadIdentity();

        if( mouseClick )
            fish.push_back( FISH() );

        glTranslatef( 0.0f, 0.0f, -1.0f );
        for( unsigned i = 0; i < fish.size(); i++ )
        {

            fish[i].fishmov();

            if( fish[i].isDead() )
            {
                fish.erase( fish.begin()+i );
                i--;

            }
            else
                fish[i].draw();

        }

        ResetKeys();
        glfwPollEvents();
        glfwSwapBuffers( window );

        std::this_thread::sleep_for( std::chrono::milliseconds(16) );
        
    }

    glfwTerminate();

}
