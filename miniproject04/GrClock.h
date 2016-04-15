#ifndef __GrClock__
#define __GrClock__

#include "GrResizableRect.h"
#include "Time.h"

#include <functional>
#include <chrono>

class GrClock : public GrResizableRect
{
    std::chrono::high_resolution_clock::time_point startTime;
    Time                                           refTime;
    bool                                           repositioning;
    Vec                                            repositioningLast;

protected:
    void drawContent();

public:
	GrClock();
    GrClock(const GrClock& plot);
    
    GrClock(const std::string& name, const Rect& rect, const Color& color);
    GrClock(const std::string& name, const Vec& origin, const Vec& size, const Color& color);
    GrClock(const std::string& name, float x, float y, float w, float h, const Color& color);
    
    GrClock(const std::string& name, const Rect& rect, float r, float g, float b, float a);
    GrClock(const std::string& name, const Vec& origin, const Vec& size, float r, float g, float b, float a);
    GrClock(const std::string& name, float x, float y, float w, float h, float r, float g, float b, float a);

    bool handle(const GlutWindow::Event& e);
};

#endif
