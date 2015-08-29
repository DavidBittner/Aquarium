#ifndef AI_FISH_H_INCLUDED
#define AI_FISH_H_INCLUDED

#include <cmath>
#include <iostream>
#include <cstdlib>
#include <GL/glfw.h>
#include <gl.h>
#include <ctime>

#include <vector>

int AQUAR_SIZEX = 800;
int AQUAR_SIZEY = 600;

static int frame = 0;

float degcos( float ang )
{
    return cos( ang * 0.0174532925 );
}
float degsin( float ang )
{
    return sin( ang * 0.0174532925 );
}

struct color
{
    color();
    color( float rr, float gg, float bb );

    float r, g, b;
    void randomize();

};

color::color()
{

    r = 1.0f;
    g = 1.0f;
    b = 1.0f;

}

color::color( float rr, float gg, float bb )
{

    r = rr;
    g = gg;
    b = bb;

}

void color::randomize()
{

    r = (rand()%1000)/1000.0f;
    g = (rand()%1000)/1000.0f;
    b = (rand()%1000)/1000.0f;

}

class FISH
{

    public:
        void draw();
        void spawn();
        void fishmov();

        bool isDead();

        FISH();

    private:
        float movang;
        float movspeed;

        float targangle;

        float xpos, ypos;
        float tarx, tary;

        float hunger;

        bool isAlive;

        std::vector< color > cols;

};

FISH::FISH()
{

    //Basic initialization, including setting the rand seed.
    srand(time(nullptr));

    movang = 90.0f;
    targangle = (rand()%360);
    movspeed = (rand()%4)+2;

    xpos = rand()%AQUAR_SIZEX;
    ypos = rand()%AQUAR_SIZEY;

    isAlive = true;

    for( int i = 0; i < 4; i++ )
    {

        cols.push_back( color() );
        cols.back().randomize();

    }

    hunger = 1.0f;

}

bool FISH::isDead()
{

    return !isAlive;

}

void FISH::draw()
{

    glPushMatrix();

    glTranslatef( xpos, ypos, 0.0f );
    glRotatef( movang, 0.0f, 0.0f, 1.0f );

    float verts[] =
    {
        0.0f, 0.0f,
        15.0f, 0.0f,
        15.0f, 15.0f,
        0.0f, 15.0f
    };

    float colors[] =
    {
        cols[0].r, cols[0].g, cols[0].b,
        cols[1].r, cols[1].g, cols[1].b,
        cols[2].r, cols[2].g, cols[2].b,
        cols[3].r, cols[3].g, cols[3].b
    };

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    glVertexPointer( 2, GL_FLOAT, 0, verts );
    glColorPointer( 3, GL_FLOAT, 0, colors );

    glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );

    glPopMatrix();

    frame++;

}

float normalizeang( float ang )
{

    while( ang > 360.0f )
        ang-=360.0f;

    while( ang < 0.0f )
        ang+=360.0f;

    return ang;

}

float getinclin( float refx, float refy, float posx, float posy )
{

    int delx = posx-refx, dely = posy-refy;
    float ret = 0.0f;

    if( delx == 0 )
    {
        if( dely < 0.0f )
            ret = 270.0f;
        else
            ret = 90.0f;
    }

    if( dely == 0 )
    {

        if( delx < 0.0f )
            ret = 90.0f;
        else
            ret = 0.0f;

    }

    if( delx!=0 && dely!=0 )
        ret = atan( dely/delx )*57.2957795;

    if( delx < 0 )
        ret = ret+90;

    return normalizeang( ret );

}

void FISH::fishmov()
{

    if( !(rand()%20) )
    {

        tarx = rand()%AQUAR_SIZEX;
        tary = rand()%AQUAR_SIZEY;

    }
    targangle = getinclin( xpos, ypos, tarx, tary );

    if( movang+720 < targangle+720 )
        movang+=4;
    else if( movang+720 > targangle+720 )
        movang-=4;

    movang = normalizeang( movang );

    if( xpos < -50.0f )
        xpos = AQUAR_SIZEX+50.0f;
    else if( xpos > AQUAR_SIZEX+50.0f )
        xpos = -50.0f;

    if( ypos < -50.0f )
        ypos = AQUAR_SIZEY+50.0f;
    else if( ypos > AQUAR_SIZEY+50.0f )
        ypos = -50.0f;

    xpos += degcos( movang )*movspeed;
    ypos += degsin( movang )*movspeed;

    hunger-=0.0005f;
    if( hunger <= 0.0f )
        isAlive = false;

}

#endif // AI_FISH_H_INCLUDED
