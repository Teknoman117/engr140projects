//
//  shader.cpp
//  forwardplus
//
//  Created by Nathaniel Lewis on 11/21/14.
//  Copyright (c) 2014 HoodooNet. All rights reserved.
//

#include <fstream>
#include <project/shader.hpp>

Shader::Shader(const std::string& path, const GLenum type)
    : handle(0), type(type)
{
    // Load the shader source code
    std::ifstream shaderFile(path.c_str());
    if(!shaderFile.is_open())
    {
        std::cerr << "[FATAL] [SHADER \"" << path << "\" ==> undetermined] Unable to open specified path" << std::endl;
        throw std::runtime_error("Unable to open specified path");
    }
    
    shaderFile.seekg ( 0, std::ios::end );
    size_t shaderSourceSize = shaderFile.tellg();
    shaderFile.seekg ( 0, std::ios::beg );
    
    std::string shaderSource;
    shaderSource.resize(shaderSourceSize);
    shaderFile.read ( &shaderSource[0], shaderSourceSize );
    shaderFile.close();
    
    const char *sourceData = shaderSource.data();
    const GLint sourceLength = (const GLint) shaderSourceSize;
    
    // Compile the shader
    handle = glCreateShader(type);
    glShaderSource(handle, 1, &sourceData, &sourceLength);
    glCompileShader(handle);
    
    // Check compilation status
    GLint r;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &r);
    if(r == GL_FALSE)
    {
        GLchar messages[256];
        glGetShaderInfoLog(handle, sizeof(messages), 0, &messages[0]);
        glDeleteShader(handle);
        handle = 0;
        
        std::cerr << "[FATAL] [SHADER \"" << path << "\" ==> " << handle << "] Compilation failed with error: " << messages << std::endl;
        throw std::runtime_error("Shader failed to compile");
    }
    
    std::cout << "[INFO] [SHADER \"" << path << "\" ==> " << handle << "] Compilation successful" << std::endl;
}

Shader::~Shader()
{
    if(IsValid())
    {
        glDeleteShader(handle);
        std::cout << "[INFO] [SHADER " << handle << "] Destroyed" << std::endl;
    }
}

bool Shader::IsValid() const
{
    return (handle > 0);
}

// Accessors
GLuint Shader::GetHandle() const
{
    return handle;
}

GLenum Shader::GetType() const
{
    return type;
}
