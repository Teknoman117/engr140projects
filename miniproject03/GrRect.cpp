#include "GrRect.h"
#include <iostream>

GrRect::GrRect()
    : Rect(), color(), inFocus(false), z(0.f), locations()
{
}

GrRect::GrRect(const GrRect& grrect)
    : Rect(grrect), color(grrect.color), inFocus(false), z(grrect.z), name(grrect.name), locations(grrect.locations)
{
}

GrRect::GrRect(const std::string& name, const Rect& rect, const Color& color)
    : Rect(rect), color(color), inFocus(false), z(0.f), name(name), locations()
{
}

GrRect::GrRect(const std::string& name, const Vec& origin, const Vec& size, const Color& color)
    : Rect(origin, size), color(color), inFocus(false), z(0.f), name(name), locations()
{
}

GrRect::GrRect(const std::string& name, float x, float y, float w, float h, const Color& color)
    : Rect(x,y,w,h), color(color), inFocus(false), z(0.f), name(name), locations()
{
}

GrRect::GrRect(const std::string& name, const Rect& rect, float r, float g, float b, float a)
    : Rect(rect), color(r,g,b,a), inFocus(false), z(0.f), name(name), locations()
{
}

GrRect::GrRect(const std::string& name, const Vec& origin, const Vec& size, float r, float g, float b, float a)
    : Rect(origin, size), color(r,g,b,a), inFocus(false), z(0.f), name(name), locations()
{
}

GrRect::GrRect(const std::string& name, float x, float y, float w, float h, float r, float g, float b, float a)
    : Rect(x,y,w,h), color(r,g,b,a), inFocus(false), z(0.f), name(name), locations()
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

const std::string& GrRect::GetName() const
{
    return name;
}

Vec GrRect::GetInWindowCoordinates(Vec global) const
{
    Vec pos = global - GetOrigin();
    pos.y = - pos.y;
    return pos;
}

Vec GrRect::GetInGlobalCoordinates(Vec local) const
{
    local.y = -local.y;
    Vec pos = local + GetOrigin();
    return pos;
}

// Methods
void GrRect::drawContent()
{   
    // Draw points
    glBegin(GL_POINTS);
    {
        glColor3d ( 1.0, 1.0, 1.0 );
        glPointSize(5);

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

void GrRect::draw()
{
    // Draw the border of the window
    glBegin( GL_LINE_STRIP );
    {
        glLineWidth(3);
        if(inFocus)
            glColor4f (0.0, 0.5, 1.0, (color.a / 2.f) + 0.5f);
        else
            glColor4f (0.7, 0.7, 0.7, (color.a / 2.f) + 0.5f);
        
        glVertex3f ( GetOrigin().x,               GetOrigin().y + 0.07,        z);
        glVertex3f ( GetOrigin().x,               GetOrigin().y - GetSize().y, z);
        glVertex3f ( GetOrigin().x + GetSize().x, GetOrigin().y - GetSize().y, z);
        glVertex3f ( GetOrigin().x + GetSize().x, GetOrigin().y + 0.07,        z);
        glVertex3f ( GetOrigin().x,               GetOrigin().y + 0.07,        z);
    }
    glEnd();

    // Draw title
    glRasterPos2f(origin.x+0.0125f, origin.y + 0.02f);
    glColor4f ( 1, 1, 1, 1 );
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char *) name.c_str());

    // Draw the title bar
    glBegin( GL_POLYGON );
    {
        glColor4f ( 0.5, 0.5, 0.5, 1 );
        glVertex3f ( GetOrigin().x,               GetOrigin().y + 0.07, z);
        glVertex3f ( GetOrigin().x,               GetOrigin().y,        z);
        glVertex3f ( GetOrigin().x + GetSize().x, GetOrigin().y,        z);
        glVertex3f ( GetOrigin().x + GetSize().x, GetOrigin().y + 0.07, z);
        glVertex3f ( GetOrigin().x,               GetOrigin().y + 0.07, z);
    }
    glEnd();

    // Draw the content
    drawContent();
}

bool GrRect::handle(const GlutWindow::Event& e)
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

    return false;
}

void GrRect::focus(bool inFocus)
{
    this->inFocus = inFocus;
}

bool GrRect::contains(const Vec& pt) const
{
    // Are we in the title region?
    if(pt.x < origin.x || pt.x > origin.x+size.x)
        return false;
    
    if(pt.y > origin.y+0.07f || pt.y < origin.y-size.y)
        return false;

    return true;
}
