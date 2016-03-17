#ifndef __GRRECT_H__
#define __GRRECT_H__

#include "Rect.h"

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

class GrRect : public Rect
{
public:
    GrRect();
    GrRect(const Rect& rect, )
};

#endif
