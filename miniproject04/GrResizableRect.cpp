#include "GrResizableRect.h"
#include <iostream>

using namespace std;

GrResizableRect::GrResizableRect()
	: GrRect(), resizing(false)
{
}

GrResizableRect::GrResizableRect(const GrResizableRect& plot)
	: GrRect(plot), resizing(false)
{
}


GrResizableRect::GrResizableRect(const std::string& name, const Rect& rect, const Color& color)
	: GrRect(name, rect, color), resizing(false)
{
}

GrResizableRect::GrResizableRect(const std::string& name, const Vec& origin, const Vec& size, const Color& color)
	: GrRect(name, origin, size, color), resizing(false)
{
}

GrResizableRect::GrResizableRect(const std::string& name, float x, float y, float w, float h, const Color& color)
	: GrRect(name, x, y, w, h, color), resizing(false)
{
}


GrResizableRect::GrResizableRect(const std::string& name, const Rect& rect, float r, float g, float b, float a)
	: GrRect(name, rect, r, g, b, a), resizing(false)
{
}

GrResizableRect::GrResizableRect(const std::string& name, const Vec& origin, const Vec& size, float r, float g, float b, float a)
    : GrRect(name, origin, size, r, g, b, a), resizing(false)
{
}

GrResizableRect::GrResizableRect(const std::string& name, float x, float y, float w, float h, float r, float g, float b, float a)
	: GrRect(name, x, y, w, h, r, g, b, a), resizing(false)
{
}

bool GrResizableRect::handle(const GlutWindow::Event& e)
{
	// Implement resize capacity
	if(e.type == GlutWindow::Motion && resizing)
    {
        size.x = e.mx;
        size.y = e.my;

        onResize();

        return true;
    }

    else if(e.type == GlutWindow::MouseDown)
    {
        // Is this in the bottom right corner?
        if(e.mx > size.x-0.033f && e.my > size.y-0.033f)
        {
            resizing = true;
            return true;
        }
    }

    else if(e.type == GlutWindow::MouseUp && resizing)
    {
        resizing = false;
        return true;
    }

	return false;
}

void GrResizableRect::onResize()
{
    
}
