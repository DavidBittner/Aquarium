
struct rect
{

    rect();

    float x, y;
    float w, h;

};

rect::rect()
{

    x = 0.0f;
    y = 0.0f;
    w = 0.0f;
    h = 0.0f;

}

bool AABB( rect a, rect b )
{

    if( a.x < b.x+b.w &&
        a.x + a.w > b.x &&
        a.y < b.y+b.h &&
        a.y + a.h > b.y )
        return true;
    else
        return false;

}
