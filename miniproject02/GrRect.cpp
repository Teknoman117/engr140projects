#include "GrRect.h"
#include <iostream>

GrRect::GrRect()
    : Rect(), color(), inFocus(false), z(0.f), locations()
{
}

GrRect::GrRect(const GrRect& grrect)
    : Rect(grrect), color(grrect.color), inFocus(false), z(0.f), locations(grrect.locations)
{
}

GrRect::GrRect(const Rect& rect, const Color& color)
    : Rect(rect), color(color), inFocus(false), z(0.f), locations()
{
}

GrRect::GrRect(const Vec& origin, const Vec& size, const Color& color)
    : Rect(origin, size), color(color), inFocus(false), z(0.f), locations()
{
}

GrRect::GrRect(float x, float y, float w, float h, const Color& color)
    : Rect(x,y,w,h), color(color), inFocus(false), z(0.f), locations()
{
}

GrRect::GrRect(const Rect& rect, float r, float g, float b, float a)
    : Rect(rect), color(r,g,b,a), inFocus(false), z(0.f), locations()
{
}

GrRect::GrRect(const Vec& origin, const Vec& size, float r, float g, float b, float a)
    : Rect(origin, size), color(r,g,b,a), inFocus(false), z(0.f), locations()
{
}

GrRect::GrRect(float x, float y, float w, float h, float r, float g, float b, float a)
    : Rect(x,y,w,h), color(r,g,b,a), inFocus(false), z(0.f), locations()
{
}

void GrRect::SetZ(float z)
{
    this->z = z;
}

float GrRect::GetZ() const
{
    return z;
}

Vec GrRect::GetInWindowCoordinates(Vec global) const
{
    Vec pos = global - GetOrigin();
    pos.y = - pos.y;
    return pos;
}

// Methods
void GrRect::draw()
{
    // Draw the border of the window
    glBegin( GL_LINE_STRIP );
    {
        if(inFocus)
            glColor4f (0.0, 0.5, 1.0, (color.a / 2.f) + 0.5f);
        else
            glColor4f (0.7, 0.7, 0.7, (color.a / 2.f) + 0.5f);
        
        glVertex3f ( GetOrigin().x,               GetOrigin().y,               z);
        glVertex3f ( GetOrigin().x,               GetOrigin().y - GetSize().y, z);
        glVertex3f ( GetOrigin().x + GetSize().x, GetOrigin().y - GetSize().y, z);
        glVertex3f ( GetOrigin().x + GetSize().x, GetOrigin().y,               z);
        glVertex3f ( GetOrigin().x,               GetOrigin().y,               z);
    }
    glEnd();
    
    // Draw points
    glBegin(GL_POINTS);
    {
        glColor3d ( 1.0, 1.0, 1.0 );
        for(std::vector<Vec>::iterator it = locations.begin(); it != locations.end(); it++)
        {
            glVertex3f(GetOrigin().x + it->x, GetOrigin().y - it->y, z);
        }
    }
    glEnd();
    
    // Draw the background of the window
    glBegin( GL_POLYGON );
    {
        glColor4f ( color.r, color.g, color.b, color.a );
        glVertex3f ( GetOrigin().x,               GetOrigin().y,               z);
        glVertex3f ( GetOrigin().x,               GetOrigin().y - GetSize().y, z);
        glVertex3f ( GetOrigin().x + GetSize().x, GetOrigin().y - GetSize().y, z);
        glVertex3f ( GetOrigin().x + GetSize().x, GetOrigin().y,               z);
        glVertex3f ( GetOrigin().x,               GetOrigin().y,               z);
    }
    glEnd();
}

void GrRect::handle(const GlutWindow::Event& e)
{
    if(e.type == GlutWindow::Motion)
    {
        std::cout << "motion: " << e.mx << " " << e.my << std::endl;
    }
    if(e.type == GlutWindow::MouseDown)
    {
        std::cout << "down: " << e.mx << " " << e.my << std::endl;
        locations.push_back(Vec(e.mx, e.my));
    }
    if(e.type == GlutWindow::MouseUp)
    {
        std::cout << "up: " << e.mx << " " << e.my << std::endl;
    }
}

void GrRect::focus(bool inFocus)
{
    this->inFocus = inFocus;
}
