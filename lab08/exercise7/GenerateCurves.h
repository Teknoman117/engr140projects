#ifndef __CURVES_H__
#define __CURVES_H__

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

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

class XYData
{
    static bool xcheck(Vec a, Vec b) {return a.x < b.x;}
    static bool ycheck(Vec a, Vec b) {return a.y < b.y;}
    
protected:
    std::vector<Vec> points;
    
public:
    void add(Vec p)
    {
        points.push_back(p);
    }
    
    Vec max()
    {
        Vec max;
        max.x = std::max_element(points.begin(), points.end(), xcheck)->x;
        max.y = std::max_element(points.begin(), points.end(), ycheck)->y;
        
        return max;
    }
    
    virtual void generate(float xini, float xend, float inc)
    {
        points.resize(0);
        for(float i = xini; i <= xend; i += inc)
            add(Vec(i,0));
    }
};

class QuadraticCurve : public XYData
{
    float a;
    float b;
    float c;
    
public:
    QuadraticCurve(float a, float b, float c)
        : a(a), b(b), c(c)
    {
    }
    
    void generate(float xini, float xend, float inc)
    {
        XYData::generate(xini, xend, inc);
        
        for(std::vector<Vec>::iterator i = points.begin(); i != points.end(); i++)
            i->y = a*std::pow(i->x, 2) + b*i->x + c;
    }
};

class CubicCurve : public XYData
{
    float a, b, c, d;
    
public:
    CubicCurve(float a, float b, float c, float d)
        : a(a), b(b), c(c), d(d)
    {
    }
    
    void generate(float xini, float xend, float inc)
    {
        XYData::generate(xini, xend, inc);
        
        for(std::vector<Vec>::iterator i = points.begin(); i != points.end(); i++)
            i->y = a*std::pow(i->x, 3) + b*std::pow(i->x, 2) + c*i->x + d;
    }
};

#endif

