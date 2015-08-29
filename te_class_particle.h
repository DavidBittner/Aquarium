#ifndef TE_CLASS_PARTICLE_H_INCLUDED
#define TE_CLASS_PARTICLE_H_INCLUDED

#include <te.h>

struct TE_PART_INFO
{

    TE_COORD cord;
    float ang;
    float alpha;
    float yvel;

};

class TE_PARTICLE_EMITTER
{

    public:
        void Create( TE_SPRITE *PartTex, int sprnum, float wid, float hei, int ampersec, int variation, int speed );
        void Draw();
        void Move( int x, int y, float ang, bool create );

    private:
        vector< TE_PART_INFO > locs;

        float angle;
        int varyam;
        int rate;
        int partspeed;

        float lifespan;

        float width, height;

        TE_SPRITE *tex;
        int spritenum;

        float lastcreate;

};

void TE_PARTICLE_EMITTER::Create( TE_SPRITE *PartTex, int sprnum, float wid, float hei, int ampersec, int variation, int speed )
{

    tex = PartTex;
    rate = ampersec;
    varyam = variation;
    partspeed = speed;

    width = wid;
    height = hei;

    spritenum = sprnum;

    lastcreate = TE_CUR_SECOND;

}

float degcos( float a )
{

    return cos( a * (pi/180.0f ) );

}

float degsin( float a )
{

    return sin( a * ( pi/180.0f ) );

}

void TE_PARTICLE_EMITTER::Move( int x, int y, float ang, bool create )
{

    if( TE_CUR_SECOND - lastcreate > 1.0f/rate and create )
    {

        locs.push_back( TE_PART_INFO() );
        locs.back().ang = ang + (( rand()%varyam )-varyam/2);
        locs.back().cord.x = x;
        locs.back().cord.y = y;
        locs.back().yvel = (degsin( locs.back().ang )*partspeed);
        locs.back().alpha = 1.0f;
        lastcreate = TE_CUR_SECOND;

    }

    for( unsigned i = 0; i < locs.size(); i++ )
    {

        locs[i].yvel -= 500.0f* TE_DELTA_TIME;
        locs[i].cord.x += (degcos( locs[i].ang )*partspeed)*TE_DELTA_TIME;
        locs[i].cord.y += locs[i].yvel*TE_DELTA_TIME;
        locs[i].alpha -= 0.01f;
        if( locs[i].alpha <= 0.0f )
        {

            locs.erase( locs.begin()+i );

        }

        if( locs[i].cord.y < 0 )
        {

            locs[i].yvel = (locs[i].yvel/1.5f)*-1;
            locs[i].ang*=-1;

        }

    }

}

void TE_PARTICLE_EMITTER::Draw()
{

    for( unsigned i = 0; i < locs.size(); i++ )
    {

        glPushAttrib( GL_CURRENT_BIT );
        glColor4f( 1.0f, 1.0f, 1.0f, locs[i].alpha );
        tex->Draw( spritenum, locs[i].cord.x, locs[i].cord.y, width, height, locs[i].ang );
        glPopAttrib();

    }

}

#endif // TE_CLASS_PARTICLE_H_INCLUDED
