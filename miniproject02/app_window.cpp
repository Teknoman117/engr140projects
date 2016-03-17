
#include <iostream>
#include <cstdlib>
#include <limits>

#include "app_window.h"

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
    : GlutWindow ( label, x, y, w, h )
{
    _markx = 0;
    _marky = 0;
    addMenuEntry ( "Option 0", evOption0 );
    addMenuEntry ( "Option 1", evOption1 );

    // Add some random windows.
    for(size_t i = 0; i < 4; i++)
    {
        float width  = (0.5f * static_cast<float>(rand()) / static_cast<float>(std::numeric_limits<int>::max())) + 0.25f;
        float height = (0.5f * static_cast<float>(rand()) / static_cast<float>(std::numeric_limits<int>::max())) + 0.25f;
        float xpos   = (1.f * static_cast<float>(rand()) / static_cast<float>(std::numeric_limits<int>::max())) - 0.5f;
        float ypos   = (1.f * static_cast<float>(rand()) / static_cast<float>(std::numeric_limits<int>::max())) - 0.5f;

        Rect r(xpos, ypos, width, height);
        windows.push_back(r);
    }

    selected = windows.end();
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

// Called every time there is a window event
void AppWindow::handle ( const Event& e )
{
    bool rd=true;

    if ( e.type==Keyboard ) 
        switch ( e.key )
        {
            // space bar
            case ' ': 
                std::cout << "Space pressed.\n";
                _markx = 1.5;
                _marky = 1.5;
                redraw();
                break;

            // Esc was pressed
            case 27:
                exit(1);
        }

    if ( e.type == MouseDown )
    {
        Vec pos = windowToScene(static_cast<float>(e.mx), static_cast<float>(e.my));
        
        for(selected = windows.begin(); selected != windows.end(); selected++)
        {
            if(selected->contains(pos))
                break;
        }

        if(selected != windows.end())
        {
            Rect r = *selected;
            windows.erase(selected);
            
            selected = windows.insert(windows.begin(), r);
            dragOffset = selected->GetOrigin() - pos ;
        }
    }

    if ( e.type == Motion)
    {
        if(selected != windows.end())
        {
            Vec pos = windowToScene(static_cast<float>(e.mx), static_cast<float>(e.my));
            pos = pos + dragOffset;

            selected->SetOrigin(pos);
        }
    }

    if ( e.type==Menu )
    {
        std::cout<<"Menu Event: "<<e.menuev<<std::endl;
        rd=false; // no need to redraw
    }

    const float incx=0.02f;
    const float incy=0.02f;

    if ( e.type==SpecialKey )
    {
        switch ( e.key )
        { 
            case GLUT_KEY_LEFT:  _markx-=incx; break;
            case GLUT_KEY_RIGHT: _markx+=incx; break;
            case GLUT_KEY_UP:    _marky+=incy; break;
            case GLUT_KEY_DOWN:  _marky-=incy; break;
            default: rd=false; // no redraw
        }
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
        Rect& r = *i;

        /* you may use GL_POLYGON for generic *convex* polygons, like this: */
        glBegin( GL_LINE_STRIP );

        if(i == selected)
            glColor3f (0.0, 0.5, 1.0);
        else
            glColor3f (0.7, 0.7, 0.7);

        glVertex3d ( r.GetOrigin().x,               r.GetOrigin().y,               static_cast<double>(i-windows.begin())/static_cast<double>(windows.size()) );
        glVertex3d ( r.GetOrigin().x,               r.GetOrigin().y-r.GetSize().y, static_cast<double>(i-windows.begin())/static_cast<double>(windows.size()) );
        glVertex3d ( r.GetOrigin().x+r.GetSize().x, r.GetOrigin().y-r.GetSize().y, static_cast<double>(i-windows.begin())/static_cast<double>(windows.size()) );
        glVertex3d ( r.GetOrigin().x+r.GetSize().x, r.GetOrigin().y,               static_cast<double>(i-windows.begin())/static_cast<double>(windows.size()) );
        glVertex3d ( r.GetOrigin().x,               r.GetOrigin().y,               static_cast<double>(i-windows.begin())/static_cast<double>(windows.size()) );
        glEnd();

        /*glBegin( GL_POLYGON );
        glColor3f ( 0.9, 0.9, 0.9 );
        glVertex3d ( r.x,     r.y,     static_cast<double>(i)/static_cast<double>(windows.count()) );
        glVertex3d ( r.x,     r.y-r.h, static_cast<double>(i)/static_cast<double>(windows.count()) );
        glVertex3d ( r.x+r.w, r.y-r.h, static_cast<double>(i)/static_cast<double>(windows.count()) );
        glVertex3d ( r.x+r.w, r.y,     static_cast<double>(i)/static_cast<double>(windows.count()) );
        glVertex3d ( r.x,     r.y,     static_cast<double>(i)/static_cast<double>(windows.count()) );
        glEnd();*/
    }

    // Swap buffers
    glFlush();         // flush the pipeline (usually not necessary)
    glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

