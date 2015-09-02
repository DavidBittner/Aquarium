#include "collision.h"
#include "ai_fish.h"

class FishPellet
{

    public:
        FishPellet( float x, float y );

        void Draw();
        rect GetPos();

    private:
        rect pos;
        Color col;

};

FishPellet::FishPellet( float x, float y ):col(0.505f, 0.301f, 0.188f )
{

    pos.x = x;
    pos.y = y;

    pos.w = 7.5f;
    pos.h = 7.5f;

}
