
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

#include "glut_window.h"
#include "RectStash.h"


// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
{ 
    enum MenuEv { evOption0, evOption1 };
    float _markx, _marky;
    int _w, _h;

    Vec                 dragOffset;

    RectStash           windows;
    RectStash::iterator selected;

public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    Vec windowToScene ( float x, float y );

private : // functions derived from the base class
    virtual void handle ( const Event& e );
    virtual void draw ();
    virtual void resize ( int w, int h );
};

#endif // APP_WINDOW_H
