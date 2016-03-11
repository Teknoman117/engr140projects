#ifndef __VEC_H__
#define __VEC_H__

#include <iostream>

class Vec
{
public:
    float x;
    float y;

    static Vec null;

    Vec() : x(0.f), y(0.f)
    {
    }
    
    Vec(float x, float y) : x(x), y(y)
    {
    }
    
    Vec& add(const Vec& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    void print()
    {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};

Vec Vec::null;

#endif

