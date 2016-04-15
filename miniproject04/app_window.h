
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

#include "glut_window.h"
#include "GrRect.h"

#include <vector>

typedef std::vector<GrRect *> RectVector;

// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
{ 
    enum MenuEv { evOption0, evOption1 };
    int _w, _h;

    Vec                  dragOffset;
    bool                 isDragging;

    RectVector           windows;
    RectVector::iterator selectedWindow;
    
    RectVector::iterator windowAtLocation(Vec p);
    void focusWindow(RectVector::iterator window);
    void unfocusWindow();

public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    Vec windowToScene ( float x, float y );

private : // functions derived from the base class
    virtual void handle ( const Event& e );
    virtual void draw ();
    virtual void resize ( int w, int h );
    virtual void idle ();
};

#endif // APP_WINDOW_H
