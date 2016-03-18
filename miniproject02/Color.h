//
//  Color.h
//  
//
//  Created by Nathaniel Lewis on 3/17/16.
//
//

#ifndef Color_h
#define Color_h

struct Color3
{
    float r;
    float g;
    float b;
    
    Color3()
        : r(0.f), g(0.f), b(0.f)
    {
    }
    
    Color3(float r, float g, float b)
        : r(r), g(g), b(b)
    {
    }
};

struct Color : public Color3
{
    float a;
    
    Color()
        : Color3(), a(1.f)
    {
    }
    
    Color(float r, float g, float b, float a)
        : Color3(r,g,b), a(a)
    {
    }
};

#endif /* Color_h */
