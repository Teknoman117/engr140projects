
#include <iostream>
#include <cstdlib>
#include <limits>

#include "app_window.h"

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
    : GlutWindow ( label, x, y, w, h )
{
    addMenuEntry ( "Option 0", evOption0 );
    addMenuEntry ( "Option 1", evOption1 );

    // Add some random windows.
    for(size_t i = 0; i < 5; i++)
    {
        Rect frame((1.f * static_cast<float>(rand()) / static_cast<float>(std::numeric_limits<int>::max())) - 0.5f,
                   (1.f * static_cast<float>(rand()) / static_cast<float>(std::numeric_limits<int>::max())) - 0.5f,
                   (0.5f * static_cast<float>(rand()) / static_cast<float>(std::numeric_limits<int>::max())) + 0.25f,
                   (0.5f * static_cast<float>(rand()) / static_cast<float>(std::numeric_limits<int>::max())) + 0.25f);

        Color color(static_cast<float>(rand()) / static_cast<float>(std::numeric_limits<int>::max()),
                    static_cast<float>(rand()) / static_cast<float>(std::numeric_limits<int>::max()),
                    static_cast<float>(rand()) / static_cast<float>(std::numeric_limits<int>::max()),
                    1.f);
        
        windows.push_back(GrRect(frame, color));
    }

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
        if(it->contains(p))
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
    GrRect win = *window;
    windows.erase(window);
    selectedWindow = windows.insert(windows.begin(), win);
    selectedWindow->focus(true);
    
    // Reestablish the z order of the windows
}

void AppWindow::unfocusWindow()
{
    if(selectedWindow == windows.end())
        return;
    
    selectedWindow->focus(false);
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
            selectedWindow->SetOrigin(epos + dragOffset);
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
            Vec local = selectedWindow->GetInWindowCoordinates(epos);
            if(local.y < 0.033f)
            {
                dragOffset = selectedWindow->GetOrigin() - epos;
                isDragging = true;
            }
            else
            {
                Event localEvent = e;
                localEvent.mx = local.x;
                localEvent.my = local.y;
                selectedWindow->handle(localEvent);
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
            Vec local = selectedWindow->GetInWindowCoordinates(epos);
            localEvent.mx = local.x;
            localEvent.my = local.y;
        }
        
        selectedWindow->handle(localEvent);
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

    // Draw a cross
    /*const double s=0.05;
    glBegin ( GL_LINES );
    glVertex2d ( _markx-s, _marky );
    glVertex2d ( _markx+s, _marky );
    glVertex2d ( _markx, _marky-s );
    glVertex2d ( _markx, _marky+s );
    glEnd();*/

    // Draw some white points
    /*glColor3d ( 1.0, 1.0, 1.0 );		
    glBegin ( GL_POINTS );
    glVertex2d ( 0.5, 0.5 );
    glVertex2d (-0.5,-0.5 );
    glVertex2d ( 0.5,-0.5 );
    glVertex2d (-0.5, 0.5 );
    glEnd();

    // Draw three overlapping triangles
    glBegin( GL_TRIANGLES ); // you may use GL_POLYGON for generic *convex* polygons
    glColor3f ( 1.0, 0.0, 0.0 ); // red
    glVertex2d (-0.7, -0.5 );
    glVertex2d ( 0.1, 0.8 );
    glVertex2d ( 0.3, 0.6 );

    glColor3f ( 0.0, 1.0, 0.0 ); // green
    glVertex3d ( -0.3, 0.7, 0.1 ); // the Z coordinate allows to control what is on top of what
    glVertex3d ( 0.6,-0.5, -0.1 );
    glVertex3d ( 0.7,-0.3, -0.1 );

    glColor3f ( 0.0, 0.0, 1.0 ); // blue
    glVertex3d ( 0.85, -0.25, 0.0 );
    glVertex3d ( -0.8, -0.4, -0.1 );
    glVertex3d ( -0.8, -0.1, -0.1 );
    glEnd();*/


    for(RectVector::iterator i = windows.begin(); i != windows.end(); i++)
    {
        i->draw();
    }

    // Swap buffers
    glFlush();         // flush the pipeline (usually not necessary)
    glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

