#ifndef __GRRECT_H__
#define __GRRECT_H__

#include "Rect.h"
#include "Color.h"

#include "glut_window.h"

#include <vector>
#include <string>

class GrRect : public Rect
{
    Color       color;
    bool        inFocus;
    float       z;
    std::string name;
    
protected:
    std::vector<Vec> locations;
    virtual void drawContent();
    
public:
    GrRect();
    GrRect(const GrRect& grrect);
    
    GrRect(const std::string& name, const Rect& rect, const Color& color);
    GrRect(const std::string& name, const Vec& origin, const Vec& size, const Color& color);
    GrRect(const std::string& name, float x, float y, float w, float h, const Color& color);
    
    GrRect(const std::string& name, const Rect& rect, float r, float g, float b, float a);
    GrRect(const std::string& name, const Vec& origin, const Vec& size, float r, float g, float b, float a);
    GrRect(const std::string& name, float x, float y, float w, float h, float r, float g, float b, float a);
    
    // Accessors
    void SetZ(float z);
    float GetZ() const;
    const std::string& GetName() const;
    
    Vec GetInWindowCoordinates(Vec global) const;
    Vec GetInGlobalCoordinates(Vec local) const;
    
    // Methods
    virtual void draw();
    virtual void focus(bool inFocus);
    virtual bool handle(const GlutWindow::Event& e);

    // need a new contains to account for title bar
    virtual bool contains(const Vec& pt) const;
};

#endif
