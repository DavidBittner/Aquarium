#include "collision.h"

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

class FishPellet
{

    public:
        FishPellet( float x, float y );

        void draw();
        rect *getPos();

    private:
        rect pos;
        color col;

};

FishPellet::FishPellet( float x, float y ):col( 0.505f, 0.301f, 0.188f )
{

    pos.x = x;
    pos.y = y;

    pos.w = 7.5f;
    pos.h = 7.5f;

}

rect *FishPellet::getPos()
{

    return &pos;

}

void FishPellet::draw()
{

	float verts[] =
	{

		pos.x, pos.y,
		pos.x+pos.w, pos.y,
		pos.x+pos.w, pos.y+pos.h,
		pos.x, pos.y+pos.h

	};

	float colors[] =
	{

		col.r, col.g, col.b,
		col.r, col.g, col.b,
		col.r, col.g, col.b,
		col.r, col.g, col.b

	};

	glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    glVertexPointer( 2, GL_FLOAT, 0, verts );
    glColorPointer( 3, GL_FLOAT, 0, colors );

    glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );

}