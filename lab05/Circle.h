#ifndef __CIRCLES_H__
#define __CIRCLES_H__

#include <cmath>

class Circle
{
	double x;
	double y;
	double r;

public:
	double area;

	Circle()
		: x(0.0), y(0.0), r(1.0)
	{
		area = M_PI * r*r;
	}

	Circle(double x, double y, double r)
		: x(x), y(y), r(r)
	{
		area = M_PI * r*r;
	}

	double getX() const
	{
		return x;
	}

	double getY() const
	{
		return y;
	}

	double getR() const
	{
		return r;
	}

	void setX(double x)
	{
		this->x = x;
	}

	void setY(double y)
	{
		this->y = y;
	}

	void setR(double r)
	{
		this->r = r;
		area = M_PI * r * r;
	}
};

#endif
