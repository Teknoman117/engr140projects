#include "GrPlot.h"
#include <iostream>

using namespace std;

GrPlot::GrPlot()
	: GrRect()
{
	equation = [] (float x) -> float { return x; };

	compute();
}

GrPlot::GrPlot(const GrPlot& plot)
	: GrRect(plot), equation(plot.equation)
{
    compute();
}


GrPlot::GrPlot(const Rect& rect, const Color& color, function<float (float)> eq)
	: GrRect(rect, color), equation(eq)
{
    compute();
}

GrPlot::GrPlot(const Vec& origin, const Vec& size, const Color& color, function<float (float)> eq)
	: GrRect(origin, size, color), equation(eq)
{
    compute();
}

GrPlot::GrPlot(float x, float y, float w, float h, const Color& color, function<float (float)> eq)
	: GrRect(x, y, w, h, color), equation(eq)
{
    compute();
}


GrPlot::GrPlot(const Rect& rect, float r, float g, float b, float a, function<float (float)> eq)
	: GrRect(rect, r, g, b, a), equation(eq)
{
    compute();
}

GrPlot::GrPlot(const Vec& origin, const Vec& size, float r, float g, float b, float a, function<float (float)> eq)
	: GrRect(origin, size, r, g, b, a), equation(eq)
{
    compute();
}

GrPlot::GrPlot(float x, float y, float w, float h, float r, float g, float b, float a, function<float (float)> eq)
	: GrRect(x, y, w, h, r, g, b, a), equation(eq)
{
    compute();
}

void GrPlot::compute()
{
	// (1000 points per float unit)
	step = (size.x * 0.5f) / 10.f;

	locations.clear();
	for(float i = -20.f; i <= 20.f; i += step)
	{
		// Get the global space point
		Vec p;
		p.x = i;
		p.y = equation(p.x);

		// Scale to window coordinates
		float scalex = size.x / 20.f;
		float scaley = size.y / 20.f;

		p.x *= scalex;
		p.y *= scaley;

		p.x += GetSize().x / 2.f;
		p.y = -p.y + GetSize().y / 2.f;

		locations.push_back(p);
		cout << p.x << " " << p.y << endl;
	}
}

void GrPlot::draw(int w, int h)
{
	glEnable(GL_SCISSOR_TEST);

	
	float llx = ((GetOrigin().x/2) + 0.5f) * (float) w;
	float lly = (((GetOrigin().y-GetSize().y)/2) + 0.5) * (float) h;
	float width = (GetSize().x / 2) * (float) w;
	float height = (GetSize().y / 2) * (float) h;

	glScissor(llx,lly,width,height);
	//glScissor(0.5f+(GetOrigin().x/2) * (float) w, 0.5f+(GetOrigin().y/2) * (float) h, (GetSize().x / 2) * (float) w, (GetSize().y/2) * (float) h);

	// draw lines
    glBegin( GL_LINE_STRIP );
    {
        glColor3d ( 1.0, 1.0, 1.0 );
        for(std::vector<Vec>::iterator it = locations.begin(); it != locations.end(); it++)
        {
            glVertex3f(GetOrigin().x + it->x, GetOrigin().y - it->y, GetZ());
        }
    }
    glEnd();

	// Call the super class method
	GrRect::draw(w, h);

	glDisable(GL_SCISSOR_TEST);
}

void GrPlot::handle(const GlutWindow::Event& e)
{

}
