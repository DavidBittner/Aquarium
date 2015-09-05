#include <GL/glfw3.h>
#include "ai_fish.h"
#include "mouse.h"

#include <chrono>
#include <thread>

std::vector<FISH> fish;
std::vector<FishPellet> fishfood;

const int SCREEN_SIZE_WID = 800;
const int SCREEN_SIZE_HEI = 600;

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

    //Basic initalization as well as setting up the screen size and so on.
    glfwInit();
    GLFWwindow *window = glfwCreateWindow( SCREEN_SIZE_WID, SCREEN_SIZE_HEI, "Test", nullptr, nullptr );
    glfwMakeContextCurrent( window );
    glfwSetWindowSizeCallback( window, Reshape );

    //Setting various callbacks for the cursor/mouse clicks.
    glfwSetCursorPosCallback( window, MousePosFunc );
    glfwSetMouseButtonCallback( window, MouseButtonFunc );
    
    //Reshaping the screen to support the new scale.
    Reshape( window, SCREEN_SIZE_WID, SCREEN_SIZE_HEI );

    //Also, setting the aquar size variables to the current size of the screen.
    glfwGetWindowSize( window, AQUAR_SIZEX, AQUAR_SIZEY );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    srand( time(nullptr) );

    for( int i = 0; i < 10; i++ )
        fish.push_back( FISH() );

    while( !glfwWindowShouldClose( window ) )
    {

        glfwGetWindowSize( window, AQUAR_SIZEX, AQUAR_SIZEY );
       
        glClear( GL_COLOR_BUFFER_BIT );
        glLoadIdentity();

        if( mouseClick )
            fishfood.push_back( FishPellet( mousex, *AQUAR_SIZEY-mousey ) );

        glTranslatef( 0.0f, 0.0f, -1.0f );
        for( unsigned i = 0; i < fish.size(); i++ )
        {

            if( fish[i].isHungry() )
            {
                for( unsigned j = 0; j < fishfood.size(); j++ )
                {

                    if( AABB( *fishfood[j].getPos(), *fish[i].getPos() ) )
                    {

                        fishfood.erase( fishfood.begin() + j );
                        j--;

                        fish[i].ate();

                    }
                }
            }

            //Give the fish a list of the food so it knows where to look.
            fish[i].getFood( fishfood );
            //Move the fish around.
            fish[i].fishmov();

            if( fish[i].isDead() )
            {
                fish.erase( fish.begin()+i );
                i--;

            }
            else
                fish[i].draw();

        }
        for( unsigned i = 0; i < fishfood.size(); i++ )
            fishfood[i].draw();

        ResetKeys();
        glfwPollEvents();
        glfwSwapBuffers( window );

        std::this_thread::sleep_for( std::chrono::milliseconds(16) );
        
    }

    glfwTerminate();

}
