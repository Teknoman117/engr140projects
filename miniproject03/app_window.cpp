
#include <iostream>
#include <cstdlib>
#include <limits>

#include "app_window.h"
#include "GrPlot.h"

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
    : GlutWindow ( label, x, y, w, h )
{
    addMenuEntry ( "Option 0", evOption0 );
    addMenuEntry ( "Option 1", evOption1 );

    // Add plot
    GrPlot *a = new GrPlot(Vec(0.125, 0.125), Vec(1.0, 1.0), Color(0.75, 0.2, 0.2, 1.0), [] (float x) -> float
    {
        return x;
    });

    GrPlot *b = new GrPlot(Vec(0.25, 0.25), Vec(1.0, 1.0), Color(0.2, 0.75, 0.2, 1.0), [] (float x) -> float
    {
        return (x*x)/2.f;
    });

    windows.push_back(a);
    windows.push_back(b);

    selectedWindow = windows.end();
    isDragging = false;
}

// mouse events are in window coordinates, but your scene is in [0,1]x[0,1],
// so make here the conversion when needed

Vec AppWindow::windowToScene ( float x, float y )
{
    Vec v;
    v.x = (2.0f*(x/float(_w))) - 1.0f;
    v.y = 1.0f - (2.0f*(y/float(_h)));
    return v;
}

// Get the window selected at a point
RectVector::iterator AppWindow::windowAtLocation(Vec p)
{
    // Figure out of the event was located inside a window
    RectVector::iterator it;
    for(it = windows.begin(); it != windows.end(); it++)
    {
        if((*it)->contains(p))
            break;
    }
    
    return it;
}

void AppWindow::focusWindow(RectVector::iterator window)
{
    if(window == selectedWindow || window == windows.end())
        return;
    
    // Unfocus the existing window
    unfocusWindow();
    
    // Move this window to the front of the queue
    GrRect *win = (*window);
    windows.erase(window);
    selectedWindow = windows.insert(windows.begin(), win);
    (*selectedWindow)->focus(true);
    
    // Reestablish the z order of the windows
}

void AppWindow::unfocusWindow()
{
    if(selectedWindow == windows.end())
        return;
    
    (*selectedWindow)->focus(false);
    selectedWindow = windows.end();
}

// Called every time there is a window event
void AppWindow::handle ( const Event& e )
{
    const Vec epos = windowToScene(static_cast<float>(e.mx), static_cast<float>(e.my));
    bool  rd       = true;
    
    // Check abort condition first
    if ( e.type==Keyboard )
        switch ( e.key )
        {
                // Esc was pressed
            case 27:
                exit(1);
        }
    
    if(isDragging)
    {
        if(e.type == Motion)
        {
            (*selectedWindow)->SetOrigin(epos + dragOffset);
        }
        else if(e.type == MouseUp)
        {
            isDragging = false;
        }
    }
    
    else if( e.type == MouseDown )
    {
        RectVector::iterator window = windowAtLocation(epos);
        
        // Focus on the selected window
        if(window != windows.end())
        {
            focusWindow(window);
            
            // Check if we should enable dragging
            Vec local = (*selectedWindow)->GetInWindowCoordinates(epos);
            if(local.y < 0.033f)
            {
                dragOffset = (*selectedWindow)->GetOrigin() - epos;
                isDragging = true;
            }
            else
            {
                Event localEvent = e;
                localEvent.mx = local.x;
                localEvent.my = local.y;
                (*selectedWindow)->handle(localEvent);
            }
        }
        
        // Perform a deselection if there is a window
        else unfocusWindow();
    }
    
    // Otherwise forward other events if
    else if( selectedWindow != windows.end())
    {
        Event localEvent = e;
        
        if(e.type == MouseUp || e.type == Motion )
        {
            Vec local = (*selectedWindow)->GetInWindowCoordinates(epos);
            localEvent.mx = local.x;
            localEvent.my = local.y;
        }
        
        (*selectedWindow)->handle(localEvent);
    }

    if (rd) redraw(); // ask the window to be rendered when possible
}

void AppWindow::resize ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );
   _w=w; _h=h;
 }

// here we will redraw the scene according to the current state of the application.
void AppWindow::draw ()
{
    // Clear the rendering window
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Clear the trasnformation stack
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Set drawing color to yellow
    glColor3d ( 1.0, 0.9, 0.3 );


    for(RectVector::iterator i = windows.begin(); i != windows.end(); i++)
    {
        (*i)->draw(_w, _h);
    }

    // Swap buffers
    glFlush();         // flush the pipeline (usually not necessary)
    glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

