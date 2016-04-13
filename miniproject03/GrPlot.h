#ifndef __GRPLOT__
#define __GRPLOT__

#include "GrRect.h"
#include <functional>

class GrPlot : public GrRect
{
	std::function<float (float)> equation;
	float                        step;
    bool                         resizing;

    Vec  getPointInGLSpace(Vec v);
	void compute();

protected:
    void drawContent();

public:
	GrPlot();
    GrPlot(const GrPlot& plot);
    
    GrPlot(const std::string& name, const Rect& rect, const Color& color, std::function<float (float)> eq);
    GrPlot(const std::string& name, const Vec& origin, const Vec& size, const Color& color, std::function<float (float)> eq);
    GrPlot(const std::string& name, float x, float y, float w, float h, const Color& color, std::function<float (float)> eq);
    
    GrPlot(const std::string& name, const Rect& rect, float r, float g, float b, float a, std::function<float (float)> eq);
    GrPlot(const std::string& name, const Vec& origin, const Vec& size, float r, float g, float b, float a, std::function<float (float)> eq);
    GrPlot(const std::string& name, float x, float y, float w, float h, float r, float g, float b, float a, std::function<float (float)> eq);

    bool handle(const GlutWindow::Event& e);
};

#endif
