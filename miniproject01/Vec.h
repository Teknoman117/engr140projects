#ifndef __VEC_H__
#define __VEC_H__

class Vec
{
public:
    float x;
    float y;

    static Vec null;

    Vec();
    Vec(float x, float y);
    
    Vec operator+(const Vec& rhs) const;
    Vec operator-(const Vec& rhs) const;
    Vec& operator=(const Vec& rhs);
    
    void print();
};

#endif

