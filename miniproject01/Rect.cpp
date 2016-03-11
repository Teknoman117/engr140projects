#include "Rect.h"

Rect::Rect() : x(-0.5f), y(-0.5f), w(1.f), h(1.f)
{
}

Rect::Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h)
{
}

bool Rect::contains(const Vec& pt) const
{
    if(pt.x < x || pt.x > x+w)
        return false;
        
    if(pt.y > y || pt.y < y-h)
        return false;
        
    return true; 
}

Vec Rect::getOrigin() const
{
    return Vec(x,y);
}

void Rect::setOrigin(Vec o)
{
    x = o.x;
    y = o.y;
}

