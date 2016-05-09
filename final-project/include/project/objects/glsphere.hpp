#ifndef SO_SPHERE_H
#define SO_SPHERE_H

// Include needed header files
#include <project/common.hpp>
#include <project/program.hpp>
#include <project/objects/globject.hpp>

class GLSphere : public GLObject
{
    size_t numPoints;
    
public:
    GLSphere(float radius, unsigned short resolution);

    void Draw();
};

#endif 
