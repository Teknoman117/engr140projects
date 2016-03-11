/*=================================================
 * gl_window 1.0
 * UC Merced, M. Kallmann 
 *=================================================*/

// Ensure the header file is included only once in multi-file projects
#ifndef GLUT_WINDOW_H
#define GLUT_WINDOW_H

// Include GLUT (OpenGL Graphics Utility Library) header files.
// In Windows we include our own included freeglut header file, but
// in Linux, the header file should be installed in the system.
#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// This is a class wrapper for the C interface of glut
// It also provides some low-level OpenGL initialization so that
// the user-derived class can concentrate on its taks and be
// more independent from the interface to OpenGL
class GlutWindow
 { public :
    enum EventType { SpecialKey, Keyboard, MouseUp, MouseDown, Motion, Menu, None };
    struct Event
     { EventType type;
       int button, key, menuev, mx, my;
       Event() { type=None; button=key=menuev=mx=my=0; }
     };

   private: // internal callback functions called by freeglut
    static void glutIdleCB ();
    static void glutMenuCB ( int m );
    static void glutMouseCB ( int b, int s, int x, int y );
    static void glutMotionCB ( int x, int y );
    static void glutDisplayCB ();
    static void glutKeyboardCB ( unsigned char key, int x, int y );
    static void glutSpecialCB ( int key, int x, int y );
    static void glutReshapeCB ( int w, int h );

   public:
    // constructor
    GlutWindow ( const char* label, int x, int y, int w, int h );

    // facilitate creation of submenus
    void addMenuEntry ( const char* label, int menuev ) { ::glutAddMenuEntry(label,menuev); }

    // asks glut to redraw the windows as soon as possible
    void redraw() { glutPostRedisplay(); }

    // Note that glutMainLoop never returns so your program is entirely event-driven
    void run () { glutMainLoop (); }

   private : // functions to be derived by the user class
    virtual void draw ()=0;
    virtual void handle ( const Event& e ) {}
    virtual void resize ( int w, int h ) {}
    virtual void idle () {}
 };

#endif // GLUT_WINDOW_H
