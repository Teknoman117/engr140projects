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

    Rect();
    Rect(float x, float y, float w, float h);
    
    bool contains(const Vec& pt) const;

    Vec getOrigin() const ;
    void setOrigin(Vec o);
};

#endif

