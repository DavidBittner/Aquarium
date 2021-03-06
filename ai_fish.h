#ifndef AI_FISH_H_INCLUDED
#define AI_FISH_H_INCLUDED

#include <cmath>
#include <iostream>
#include <cstdlib>
#include <GL/glfw3.h>
#include <ctime>

#include <vector>

#include "class_fishfood.h"

//Width and height of the 'aquarium'.
int *AQUAR_SIZEX = new int;
int *AQUAR_SIZEY = new int;

static int frame = 0;

//Two functions for calculating cos and sin with degrees instead of radians.
float degcos( float ang )
{
    return cos( ang * 0.0174532925 );
}
float degsin( float ang )
{
    return sin( ang * 0.0174532925 );
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

class FISH
{

    public:
        void draw();
        void spawn();
        void fishmov();

        bool isDead();
        bool isHungry();

        void getFood( std::vector<FishPellet> foodlocs );

        void ate();

        rect *getPos();

        FISH();

    private:
        //The movement angle and speed.
        float movang;
        float movspeed;

        //The angle the fish is trying to reach.
        float targangle;

        //The rect that has it's size/position.
        rect fishrect;
        float tarx, tary;

        //Hunger related vars.
        float hunger;
        bool isfood;
        bool ishungry;

        bool isAlive;

        rect *foodtarget;

        std::vector< color > cols;

};

FISH::FISH()
{

    //Basic initialization.

    movang = 90.0f;
    targangle = (rand()%360);
    movspeed = (rand()%4)+2;

    fishrect.x = rand()%*AQUAR_SIZEX;
    fishrect.y = rand()%*AQUAR_SIZEY;

    fishrect.w = 15.0f;
    fishrect.h = 15.0f;

    isAlive = true;

    for( int i = 0; i < 4; i++ )
    {

        cols.push_back( color() );
        cols.back().randomize();

    }

    hunger = 1.0f;

}

void FISH::ate()
{

    hunger = 1.0f;

}

rect *FISH::getPos()
{

    return &fishrect;

}

bool FISH::isHungry()
{

    //Simply check whether or not it's hungry.
    if( hunger <= 0.5f )
    {

        ishungry = true;
        return true;

    }
    else
    {

        ishungry = false;
        return false;

    }

}

float getDist( float ax, float ay, float bx, float by )
{

    return sqrt( pow( bx - ax, 2 ) + pow( by - ay, 2 ) );

}

void FISH::getFood( std::vector<FishPellet> foodlocs )
{

    //Variable that holds the shortest distance calculated.
    float lowestdist = 0.0f;

    if( foodlocs.size() > 0 )
    {

        foodtarget = foodlocs[0].getPos();
        lowestdist = getDist( fishrect.x, fishrect.y, foodlocs[0].getPos()->x, foodlocs[0].getPos()->y );

    }

    //For loop that runs through the available food locations.
    for( unsigned i = 0; i < foodlocs.size(); i++ )
    {

        //The distance away from the current food pellet.
        float curdist = getDist( fishrect.x, fishrect.y, foodlocs[i].getPos()->x, foodlocs[i].getPos()->y );

        //If the current distance is less than the lowest distance then the lowest distance is now the current distance.
        if( curdist < lowestdist )
        {

            lowestdist = curdist;
            foodtarget = foodlocs[i].getPos();

        }

    }

    if( !foodlocs.size() )
    {

        isfood = false;

    }
    else
    {

        isfood = true;

    }

}

bool FISH::isDead()
{

    return !isAlive;

}

void FISH::draw()
{

    glPushMatrix();

    glTranslatef( fishrect.x, fishrect.y, 0.0f );
    glRotatef( movang, 0.0f, 0.0f, 1.0f );

    float verts[] =
    {
        0.0f, 0.0f,
        fishrect.w, 0.0f,
        fishrect.w, fishrect.h,
        0.0f, fishrect.h
    };

    float colors[] =
    {
        cols[0].r, cols[0].g, cols[0].b, hunger,
        cols[1].r, cols[1].g, cols[1].b, hunger,
        cols[2].r, cols[2].g, cols[2].b, hunger,
        cols[3].r, cols[3].g, cols[3].b, hunger
    };

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    glVertexPointer( 2, GL_FLOAT, 0, verts );
    glColorPointer( 4, GL_FLOAT, 0, colors );

    glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );

    glPopMatrix();

    frame++;

}

void FISH::fishmov()
{

    if( !(rand()%20) && !ishungry )
    {

        tarx = rand()%*AQUAR_SIZEX;
        tary = rand()%*AQUAR_SIZEY;

    }else if( isfood && ishungry )
    {

        tarx = foodtarget->x;
        tary = foodtarget->y;

    }
    targangle = getinclin( fishrect.x, fishrect.y, tarx, tary );

    if( movang+720 < targangle+720 )
        movang+=4;
    else if( movang+720 > targangle+720 )
        movang-=4;

    movang = normalizeang( movang );

    if( fishrect.x < -50.0f )
        fishrect.x = *AQUAR_SIZEX+50.0f;
    else if( fishrect.x > *AQUAR_SIZEX+50.0f )
        fishrect.x = -50.0f;

    if( fishrect.y < -50.0f )
        fishrect.y = *AQUAR_SIZEY+50.0f;
    else if( fishrect.y > *AQUAR_SIZEY+50.0f )
        fishrect.y = -50.0f;

    fishrect.x += degcos( movang )*movspeed;
    fishrect.y += degsin( movang )*movspeed;

    hunger-=0.0005f;
    if( hunger <= 0.0f )
        isAlive = false;

}

#endif // AI_FISH_H_INCLUDED
