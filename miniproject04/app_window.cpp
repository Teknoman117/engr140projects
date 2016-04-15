
#include <iostream>
#include <cstdlib>
#include <limits>
#include <cmath>
#include <algorithm>

#include "app_window.h"
#include "GrPlot.h"
#include "GrClock.h"

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
    : GlutWindow ( label, x, y, w, h )
{
    // Add plot
    windows.push_back(new GrPlot("y=x^3", Vec(-0.85, 0.85), Vec(1.0, 1.0), Color(0.75, 0.2, 0.2, 1.0), [] (float x) -> float
    {
        const float a = 1.f;
        const float b = 0.f;
        const float c = 0.f;
        const float d = 0.f;
        return a*pow(x,3) + b*pow(x,2) + c*x + d;
    }));
    windows.push_back(new GrPlot("y=.5x^3 + x^2", Vec(-0.75, 0.75), Vec(1.0, 1.0), Color(0.2, 0.75, 0.2, 1.0), [] (float x) -> float
    {
        const float a = 0.5f;
        const float b = 1.f;
        const float c = 0.f;
        const float d = 0.f;
        return a*pow(x,3) + b*pow(x,2) + c*x + d;
    }));
    windows.push_back(new GrPlot("y=.25x^3+.75x^2+2x-3", Vec(-0.65, 0.65), Vec(1.0, 1.0), Color(0.2, 0.2, 0.75, 1.0), [] (float x) -> float
    {
        const float a = 0.25f;
        const float b = 0.75f;
        const float c = 2.f;
        const float d = -3.f;
        return a*pow(x,3) + b*pow(x,2) + c*x + d;
    }));
    windows.push_back(new GrPlot("y=x^2", Vec(-0.55, 0.55), Vec(1.0, 1.0), Color(0.75, 0.75, 0.2, 1.0), [] (float x) -> float
    {
        const float a = 1.f;
        const float b = 0.f;
        const float c = 0.f;
        return a*pow(x,2) + b*x + c;
    }));
    windows.push_back(new GrPlot("y=.5x^2+x+1", Vec(-0.45, 0.45), Vec(1.0, 1.0), Color(0.2, 0.75, 0.75, 1.0), [] (float x) -> float
    {
        const float a = 0.5f;
        const float b = 1.f;
        const float c = 1.f;
        return a*pow(x,2) + b*x + c;
    }));
    windows.push_back(new GrPlot("y=.25x^2+.75x-2", Vec(-0.35, 0.35), Vec(1.0, 1.0), Color(0.75, 0.2, 0.75, 1.0), [] (float x) -> float
    {
        const float a = 0.25f;
        const float b = 0.75f;
        const float c = -2.f;
        return a*pow(x,2) + b*x + c;
    }));
    windows.push_back(new GrClock("Clock", Vec(-0.25, 0.25), Vec(1.0, 1.0), Color(0.2, 0.2, 0.2, 1.0)));

    std::hash<GrRect *> hasher;
    for_each(windows.begin(), windows.end(), [&] (GrRect *w)
    {
        addMenuEntry ( w->GetName().c_str(), hasher(w));
    });

    selectedWindow = windows.end();
    focusWindow(windows.end()-1);
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
            
            // Check if we should enable dragging (mouse in title bar)
            Vec local = (*selectedWindow)->GetInWindowCoordinates(epos);
            if(local.y <= 0.0)
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

    else if( e.type == Menu )
    {
        std::hash<GrRect *> hasher;
        focusWindow(std::find_if(windows.begin(), windows.end(), [&] (GrRect *w) -> bool
        {
            return e.menuev == (int) hasher(w);
        }));
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
    glEnable ( GL_SCISSOR_TEST );
    glEnable ( GL_LINE_SMOOTH );

    // Clear the trasnformation stack
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Set drawing color to yellow
    glColor3d ( 1.0, 0.9, 0.3 );

    for(RectVector::iterator i = windows.begin(); i != windows.end(); i++)
    {
        GrRect *window = (*i);

        // Compute the lower left corner of the window
        Vec dimensions(_w, _h);
        Vec size = ((window->GetSize() + Vec(0,0.07)) / 2.f) * dimensions;

        Vec lowerLeft = Vec(((window->GetOrigin().x / 2.f) + 0.5f),
                            (((window->GetOrigin().y - window->GetSize().y) / 2.f) + 0.5f))
                        * dimensions;

        // Restrict the drawing area of the window to it's region
        glScissor(lowerLeft.x-1, lowerLeft.y-1, size.x+3, size.y+3);
        window->draw();
    }

    // Swap buffers
    glDisable(GL_SCISSOR_TEST);

    glFlush();         // flush the pipeline (usually not necessary)
    glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

void AppWindow::idle()
{
    redraw();
}

