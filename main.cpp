#include <GL/glfw.h>
#include <te.h>
#include "ai_fish.h"

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

void createFishies()
{

    for(;;)
    {
        std::this_thread::sleep_for( std::chrono::seconds(1) );
        fish.push_back( FISH() );
    }
}

int main()
{

    std::thread create( createFishies );

    glfwInit();
    GLFWwindow *window = glfwCreateWindow( AQUAR_SIZEX, AQUAR_SIZEY, "Test", nullptr, nullptr );
    glfwMakeContextCurrent( window );
    glfwSetWindowSizeCallback( window, Reshape );

    Reshape( window, AQUAR_SIZEX, AQUAR_SIZEY );

    while( !glfwWindowShouldClose( window ) )
    {

        glClear( GL_COLOR_BUFFER_BIT );
        glLoadIdentity();

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

        glfwPollEvents();
        glfwSwapBuffers( window );

        std::this_thread::sleep_for( std::chrono::milliseconds(16) );

    }

    glfwTerminate();

}
