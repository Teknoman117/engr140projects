#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include <iostream>
#include "Sortable.h"

class Circle : public Sortable
{
    float radius;

public:
    Circle() : radius(0)
    {
    }

    Circle(float r) : radius(r)
    {
    }
  
    void print()
    {
        std::cout << "Circle with radius: " << radius << std::endl;
    }

    bool compare(const Sortable *s)
    {
        const Circle *other = dynamic_cast<const Circle *>(s);

        return radius < other->radius;
    }
};

#endif

