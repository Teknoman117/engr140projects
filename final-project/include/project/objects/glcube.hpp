#ifndef __GLSKYBOX_HPP__
#define __GLSKYBOX_HPP__

#include <project/common.hpp>
#include <project/objects/globject.hpp>

class GLCube : public GLObject
{
public:
    GLCube();
    virtual ~GLCube();

    void Draw();
};

#endif
