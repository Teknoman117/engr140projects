#ifndef LA10_Mat_h
#define LA10_Mat_h

#include "Vec.h"

using namespace std;

class Mat
{
public:
    float data[4];

    static Mat Identity;

    Mat(float a, float b, float c, float d)
    {
        data[0] = a;
        data[1] = b;
        data[2] = c;
        data[3] = d;
    }

    Mat operator + (const Mat & m)
    {
        return Mat(data[0] + m.data[0], 
                   data[1] + m.data[1],
                   data[2] + m.data[2],
                   data[3] + m.data[3]);
    }

    Mat operator - (const Mat & m)
    {
        return Mat(data[0] - m.data[0], 
                   data[1] - m.data[1],
                   data[2] - m.data[2],
                   data[3] - m.data[3]);
    }

    Mat operator * (float f)
    {
        return Mat(data[0] + f, 
                   data[1] + f,
                   data[2] + f,
                   data[3] + f);
    }

    Mat operator * (const Mat& m)
    {
        return Mat(data[0] * m.data[0] + data[1] * m.data[2], 
                   data[0] * m.data[1] + data[1] * m.data[3],
                   data[2] * m.data[0] + data[3] * m.data[2], 
                   data[2] * m.data[1] + data[3] * m.data[3]);
    }
};

Mat operator * (float f, const Mat& m)
{
    return Mat(m.data[0] * f, 
               m.data[1] * f,
               m.data[2] * f,
               m.data[3] * f);
}

Vec operator * (const Vec& v, const Mat& m)
{
    return Vec(v.x * m.data[0] + v.y * m.data[2],
               v.x * m.data[1] + v.y * m.data[3]);
}

Mat Mat::Identity(1.0f, 0.0f, 0.0f, 1.0f);

#endif
