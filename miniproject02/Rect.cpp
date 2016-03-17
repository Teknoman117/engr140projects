#include "Rect.h"

Rect::Rect()
    : origin(-0.5f, -0.5f), size(1.f, 1.f)
{
}

// Forward through
Rect::Rect(const Rect& rect)
    : origin(rect.origin), size(rect.size)
{
}

Rect::Rect(float x, float y, float w, float h)
    : origin(x,y), size(w,h)
{
}

Rect::Rect(const Vec& origin, const Vec& size)
    : origin(origin), size(size)
{
}

// Accessors
const Vec& Rect::GetOrigin() const
{
    return origin;
}

const Vec& Rect::GetSize() const
{
    return size;
}

// Mutators
void Rect::SetOrigin(const Vec& origin)
{
    this->origin = origin;
}

void Rect::SetSize(const Vec& size)
{
    this->size = size;
}

// Methods
bool Rect::contains(const Vec& pt) const
{
    if(pt.x < origin.x || pt.x > origin.x+size.x)
        return false;
    
    if(pt.y > origin.y || pt.y < origin.y-size.y)
        return false;
    
    return true;
}
