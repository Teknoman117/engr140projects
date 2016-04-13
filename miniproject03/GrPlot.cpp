#include "GrPlot.h"
#include <iostream>

using namespace std;

GrPlot::GrPlot()
	: GrRect(), resizing(false)
{
	equation = [] (float x) -> float { return x; };

	compute();
}

GrPlot::GrPlot(const GrPlot& plot)
	: GrRect(plot), equation(plot.equation), resizing(false)
{
    compute();
}


GrPlot::GrPlot(const std::string& name, const Rect& rect, const Color& color, function<float (float)> eq)
	: GrRect(name, rect, color), equation(eq), resizing(false)
{
    compute();
}

GrPlot::GrPlot(const std::string& name, const Vec& origin, const Vec& size, const Color& color, function<float (float)> eq)
	: GrRect(name, origin, size, color), equation(eq), resizing(false)
{
    compute();
}

GrPlot::GrPlot(const std::string& name, float x, float y, float w, float h, const Color& color, function<float (float)> eq)
	: GrRect(name, x, y, w, h, color), equation(eq), resizing(false)
{
    compute();
}


GrPlot::GrPlot(const std::string& name, const Rect& rect, float r, float g, float b, float a, function<float (float)> eq)
	: GrRect(name, rect, r, g, b, a), equation(eq), resizing(false)
{
    compute();
}

GrPlot::GrPlot(const std::string& name, const Vec& origin, const Vec& size, float r, float g, float b, float a, function<float (float)> eq)
	: GrRect(name, origin, size, r, g, b, a), equation(eq), resizing(false)
{
    compute();
}

GrPlot::GrPlot(const std::string& name, float x, float y, float w, float h, float r, float g, float b, float a, function<float (float)> eq)
	: GrRect(name, x, y, w, h, r, g, b, a), equation(eq), resizing(false)
{
    compute();
}

Vec GrPlot::getPointInGLSpace(Vec v)
{
	v.y = -v.y;
	return (v * (size / 20.f)) + (size / 2.f);
}

void GrPlot::compute()
{
	// (1000 points per float unit)
	float step = 1.f / (2.f * size.x);

	locations.clear();
	for(float i = -10.f; i <= 10.f; i += step)
	{
		locations.push_back(getPointInGLSpace(Vec(i, equation(i))));
	}
}

void GrPlot::drawContent()
{
	// Draw the line representing the curve
    glLineWidth(1);
    glBegin( GL_LINE_STRIP );
    {
        glColor3d ( 1.0, 1.0, 1.0 );

        for(std::vector<Vec>::iterator it = locations.begin(); it != locations.end(); it++)
        {
            glVertex3f(origin.x + it->x, origin.y - it->y, GetZ());
        }
    }
    glEnd();

    // Render the axes
    glLineWidth(2);
    glBegin( GL_LINES );
    {
    	glVertex3f(origin.x, origin.y - size.y/2.f, GetZ()); glVertex3f(origin.x+size.x, origin.y - size.y/2.f, GetZ());
    	glVertex3f(origin.x + size.x/2.f, origin.y, GetZ()); glVertex3f(origin.x + size.x/2.f, origin.y - size.y, GetZ());

    	for(float x = -10.f; x <= 10.f; x+= 1.f)
    	{
    		// Coordinates
    		Vec a(x, -0.25f);
    		Vec b(x, +0.25f);
    		a = getPointInGLSpace(a);
    		b = getPointInGLSpace(b);

    		glVertex3f(origin.x + a.x, origin.y - a.y, GetZ());
    		glVertex3f(origin.x + b.x, origin.y - b.y, GetZ());
    	}

    	for(float y = -10.f; y <= 10.f; y+= 1.f)
    	{
    		// Coordinates
    		Vec a(-0.25f, y);
    		Vec b(+0.25f, y);
    		a = getPointInGLSpace(a);
    		b = getPointInGLSpace(b);

    		glVertex3f(origin.x + a.x, origin.y - a.y, GetZ());
    		glVertex3f(origin.x + b.x, origin.y - b.y, GetZ());
    	}
    }
    glEnd();

	// Call the super class method
	GrRect::drawContent();
}

bool GrPlot::handle(const GlutWindow::Event& e)
{
	// Implement resize capacity
	if(e.type == GlutWindow::Motion && resizing)
    {
        size.x = e.mx;
        size.y = e.my;
        compute();

        return true;
    }
    else if(e.type == GlutWindow::MouseDown)
    {
    	// Is this in the bottom right corner?
    	if(e.mx > size.x-0.033f && e.my > size.y-0.033f)
		{
			resizing = true;
		}
    }
    else if(e.type == GlutWindow::MouseUp)
    {
        resizing = false;
    }

	return false;
}
