#ifndef __RECT_H__
#define __RECT_H__

#include "Vec.h"

class Rect
{
public:
    float x;
    float y;
    float w;
    float h;

    Rect() : x(-0.5f), y(-0.5f), w(1.f), h(1.f)
    {
    }
    
    Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h)
    {
    }
    
    bool contains(const Vec& pt)
    {
        if(pt.x < x || pt.x > x+w)
            return false;
            
        if(pt.y > y || pt.y < y-h)
            return false;
            
        return true; 
    }
};

#endif

