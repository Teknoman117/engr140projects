#include "Vec.h"
#include <iostream>

Vec Vec::null;

Vec::Vec() : x(0.f), y(0.f)
{
}

Vec::Vec(float x, float y) : x(x), y(y)
{
}

Vec Vec::operator+(const Vec& rhs) const
{
	return Vec(x+rhs.x, y+rhs.y);
}

Vec Vec::operator-(const Vec& rhs) const
{
	return Vec(x-rhs.x, y-rhs.y);
}

Vec Vec::operator*(const Vec& rhs) const
{
	return Vec(x*rhs.x, y*rhs.y);
}

Vec Vec::operator/(const Vec& rhs) const
{
	return Vec(x/rhs.x, y/rhs.y);
}

Vec Vec::operator*(float rhs) const
{
	return Vec(x*rhs, y*rhs);
}

Vec Vec::operator/(float rhs) const
{
	return Vec(x/rhs, y/rhs);
}

Vec& Vec::operator=(const Vec& rhs)
{
	x = rhs.x;
	y = rhs.y;
	return *this;
}

void Vec::print()
{
    std::cout << "(" << x << ", " << y << ")" << std::endl;
}
