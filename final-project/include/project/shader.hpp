#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <project/common.hpp>

class Shader
{
    GLuint handle;
    GLenum type;
    
public:
    Shader(const std::string& path, const GLenum type);
    ~Shader();
    
    bool   IsValid()   const;
    
    // Accessors
    GLuint GetHandle() const;
    GLenum GetType()   const;
};

#endif
