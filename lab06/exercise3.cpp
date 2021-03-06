#include <iostream>
#include <vector>

using namespace std;

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
    
    bool contains(const Vec& pt) const
    {
        if(pt.x < x || pt.x > x+w)
            return false;
            
        if(pt.y > y || pt.y < y-h)
            return false;
            
        return true; 
    }
};

typedef vector<Rect> RectStash;

int main (int argc, char **argv)
{
    RectStash rectangles;
    Rect rect;
    while(1)
    {
        cin >> rect.x >> rect.y >> rect.w >> rect.h;
        
        if(rect.x < 0.f && rect.y < 0.f && rect.w < 0.f && rect.h < 0.f)
            break;
            
        rectangles.push_back(rect);
    }
    
    Vec vec;
    while(1)
    {
        cin >> vec.x >> vec.y;
        
        if(vec.x == -99.f && vec.y == -99.f)
            break;
            
        for(RectStash::const_iterator it = rectangles.begin(); it != rectangles.end(); it++)
        {
            if(it->contains(vec))
                cout << "in ";
            else
                cout << "out ";
        }
        cout << endl;
    }

    return 0;
}

