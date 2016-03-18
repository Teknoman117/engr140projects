#ifndef __GRRECT_H__
#define __GRRECT_H__

#include "Rect.h"
#include "Color.h"

#include "glut_window.h"

#include <vector>

class GrRect : public Rect
{
    Color color;
    bool  inFocus;
    float z;
    
    std::vector<Vec> locations;
    
public:
    GrRect();
    GrRect(const GrRect& grrect);
    
    GrRect(const Rect& rect, const Color& color);
    GrRect(const Vec& origin, const Vec& size, const Color& color);
    GrRect(float x, float y, float w, float h, const Color& color);
    
    GrRect(const Rect& rect, float r, float g, float b, float a);
    GrRect(const Vec& origin, const Vec& size, float r, float g, float b, float a);
    GrRect(float x, float y, float w, float h, float r, float g, float b, float a);
    
    // Accessors
    void SetZ(float z);
    float GetZ() const;
    
    Vec GetInWindowCoordinates(Vec global) const;
    
    // Methods
    virtual void draw();
    virtual void focus(bool inFocus);
    virtual void handle(const GlutWindow::Event& e);
};

#endif
