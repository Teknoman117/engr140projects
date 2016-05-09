#ifndef __OBJECTS_GLLINE_HPP__
#define __OBJECTS_GLLINE_HPP__

#include <project/common.hpp>
#include <project/objects/globject.hpp>

class GLLine : GLObject
{
    size_t points;
    
public:
    GLLine(const std::vector<glm::vec3>& coordinates);

    void Draw();
};

#endif
