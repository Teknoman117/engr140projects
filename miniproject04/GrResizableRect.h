#ifndef __GRRESIZABLERECT__
#define __GRRESIZABLERECT__

#include "GrRect.h"
#include <functional>

class GrResizableRect : public GrRect
{
    bool resizing;

protected:
    virtual void onResize();

public:
	GrResizableRect();
    GrResizableRect(const GrResizableRect& plot);
    
    GrResizableRect(const std::string& name, const Rect& rect, const Color& color);
    GrResizableRect(const std::string& name, const Vec& origin, const Vec& size, const Color& color);
    GrResizableRect(const std::string& name, float x, float y, float w, float h, const Color& color);
    
    GrResizableRect(const std::string& name, const Rect& rect, float r, float g, float b, float a);
    GrResizableRect(const std::string& name, const Vec& origin, const Vec& size, float r, float g, float b, float a);
    GrResizableRect(const std::string& name, float x, float y, float w, float h, float r, float g, float b, float a);

    virtual bool handle(const GlutWindow::Event& e);
};

#endif
