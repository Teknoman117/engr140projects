#ifndef __GRPLOT__
#define __GRPLOT__

#include "GrRect.h"
#include <functional>

class GrPlot : public GrRect
{
	std::function<float (float)> equation;
	float                        step;

	void compute();

public:
	GrPlot();
    GrPlot(const GrPlot& plot);
    
    GrPlot(const Rect& rect, const Color& color, std::function<float (float)> eq);
    GrPlot(const Vec& origin, const Vec& size, const Color& color, std::function<float (float)> eq);
    GrPlot(float x, float y, float w, float h, const Color& color, std::function<float (float)> eq);
    
    GrPlot(const Rect& rect, float r, float g, float b, float a, std::function<float (float)> eq);
    GrPlot(const Vec& origin, const Vec& size, float r, float g, float b, float a, std::function<float (float)> eq);
    GrPlot(float x, float y, float w, float h, float r, float g, float b, float a, std::function<float (float)> eq);

    void draw(int w, int h);
    void handle(const GlutWindow::Event& e);
};

#endif
