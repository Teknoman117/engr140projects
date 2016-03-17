#ifndef __RECT_H__
#define __RECT_H__

#include "Vec.h"

class Rect
{
    Vec origin;
    Vec size;
    
public:
    Rect();
    Rect(const Rect& rect);
    Rect(float x, float y, float w, float h);
    Rect(const Vec& origin, const Vec& size);

    // Accessors
    const Vec& GetOrigin() const;
    const Vec& GetSize() const;
    
    // Mutators
    void SetOrigin(const Vec& origin);
    void SetSize(const Vec& size);
    
    // Methods
    bool contains(const Vec& pt) const;
};

#endif

