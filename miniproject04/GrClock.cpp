#include "GrClock.h"
#include "Time.h"
#include <iostream>

using namespace std;
using namespace std::chrono;

GrClock::GrClock()
	: GrResizableRect(), startTime(high_resolution_clock::now()), repositioning(false)
{
}

GrClock::GrClock(const GrClock& plot)
	: GrResizableRect(plot), startTime(high_resolution_clock::now()), repositioning(false)
{
}


GrClock::GrClock(const std::string& name, const Rect& rect, const Color& color)
	: GrResizableRect(name, rect, color), startTime(high_resolution_clock::now()), repositioning(false)
{
}

GrClock::GrClock(const std::string& name, const Vec& origin, const Vec& size, const Color& color)
	: GrResizableRect(name, origin, size, color), startTime(high_resolution_clock::now()), repositioning(false)
{
}

GrClock::GrClock(const std::string& name, float x, float y, float w, float h, const Color& color)
	: GrResizableRect(name, x, y, w, h, color), startTime(high_resolution_clock::now()), repositioning(false)
{
}


GrClock::GrClock(const std::string& name, const Rect& rect, float r, float g, float b, float a)
	: GrResizableRect(name, rect, r, g, b, a), startTime(high_resolution_clock::now()), repositioning(false)
{
}

GrClock::GrClock(const std::string& name, const Vec& origin, const Vec& size, float r, float g, float b, float a)
	: GrResizableRect(name, origin, size, r, g, b, a), startTime(high_resolution_clock::now()), repositioning(false)
{
}

GrClock::GrClock(const std::string& name, float x, float y, float w, float h, float r, float g, float b, float a)
	: GrResizableRect(name, x, y, w, h, r, g, b, a), startTime(high_resolution_clock::now()), repositioning(false)
{
}

void GrClock::drawContent()
{
    high_resolution_clock::time_point current = high_resolution_clock::now();
    duration<float> deltaTime = duration_cast<duration<float>>(current - startTime);
    Time currentTime(0,0,deltaTime.count());

    // Compute the radius of the clock
    float r = ((size.x > size.y) ? size.y : size.x) / 2.2f;
    float rInner = r / 1.2f;
    float rOuter = r / 1.1f;

    // Draw the hands
    glLineWidth(1);
    glBegin( GL_LINES );
    {
        float h, m, s;
        Time t = refTime;
        t.add(currentTime);
        t.getAngles(h, m, s);

        // seconds
        glColor3d ( 1.0, 0.0, 0.0 );
        glVertex3f(origin.x + (size.x/2.f), 
                   origin.y - (size.y/2.f), 
                   GetZ());

        glVertex3f(origin.x + (size.x/2.f) + (r/1.1f)*cos(s), 
                   origin.y - (size.y/2.f) + (r/1.1f)*sin(s), 
                   GetZ());

        // minutes
        glColor3d ( 0.0, 1.0, 0.0 );
        glVertex3f(origin.x + (size.x/2.f), 
                   origin.y - (size.y/2.f), 
                   GetZ());

        glVertex3f(origin.x + (size.x/2.f) + (r/1.2f)*cos(m), 
                   origin.y - (size.y/2.f) + (r/1.2f)*sin(m), 
                   GetZ());

        // minutes
        glColor3d ( 0.0, 0.0, 1.0 );
        glVertex3f(origin.x + (size.x/2.f), 
                   origin.y - (size.y/2.f), 
                   GetZ());

        glVertex3f(origin.x + (size.x/2.f) + (r/2.f)*cos(h), 
                   origin.y - (size.y/2.f) + (r/2.f)*sin(h), 
                   GetZ());
    }
    glEnd();

	// Draw the outside of the clock
    glLineWidth(1);
    glBegin( GL_LINE_STRIP );
    {
        glColor3d ( 1.0, 1.0, 1.0 );

        const float step = M_PI/30.f;
        for(size_t i = 0; i <= 60; i++)
        {
            glVertex3f(origin.x + (size.x/2.f) + r*cos(step * (float) i), 
                       origin.y - (size.y/2.f) + r*sin(step * (float) i), 
                       GetZ());
        }
    }
    glEnd();

    // Draw the ticks of the clock
    glLineWidth(3);
    glBegin( GL_LINES );
    {
        glColor3d ( 1.0, 1.0, 0.0 );

        const float step = M_PI/6.f;
        for(size_t i = 0; i < 12; i++)
        {
            glVertex3f(origin.x + (size.x/2.f) + rInner*cos(step * (float) i), 
                       origin.y - (size.y/2.f) + rInner*sin(step * (float) i), 
                       GetZ());

            glVertex3f(origin.x + (size.x/2.f) + rOuter*cos(step * (float) i), 
                       origin.y - (size.y/2.f) + rOuter*sin(step * (float) i), 
                       GetZ());
        }
    }
    glEnd();

	// Call the super class method
	GrResizableRect::drawContent();
}

bool GrClock::handle(const GlutWindow::Event& e)
{
    // only process the event if the superclass did not consume it
    if(!GrResizableRect::handle(e))
    {
        // Implement resize capacity
        if(e.type == GlutWindow::Motion && repositioning)
        {
            // reposition the reference time
            Vec p = Vec(e.mx, e.my);
            Vec delta = repositioningLast - p;
            repositioningLast = p;

            refTime.addSeconds(-delta.x * 400.f);

            return true;
        }

        else if(e.type == GlutWindow::MouseDown)
        {
            repositioning = true;
            return true;
        }

        else if(e.type == GlutWindow::MouseUp && repositioning)
        {
            repositioning = false;
            return true;
        }
    }

    return false;
}
