//
//  program.cpp
//  forwardplus
//
//  Created by Nathaniel Lewis on 11/21/14.
//  Copyright (c) 2014 HoodooNet. All rights reserved.
//

#include <project/program.hpp>

Program::Program()
    : handle(glCreateProgram()), linked(false)
{
}

Program::~Program()
{
    if(IsValid())
    {
        glDeleteProgram(handle);
        std::cout << "[INFO] [PROGRAM " << handle << "] Destroyed" << std::endl;
    }
}

bool Program::IsValid() const
{
    return (handle > 0);
}

bool Program::IsLinked() const
{
    return linked;
}

GLuint Program::GetHandle() const
{
    return handle;
}

Program& Program::Attach(const std::string& path, const GLenum type)
{
    return Attach(std::make_shared<Shader>(path, type));
}

Program& Program::Attach(const std::shared_ptr<Shader>& shader)
{
    if(!shader->IsValid())
    {
        std::cerr << "[WARNING] [PROGRAM " << handle << "] attempted to attach invalid shader" << std::endl;
        return *this;
    }
   
    glAttachShader(handle, shader->GetHandle());
    shaders.push_back(shader);
    
    // Return a reference to ourself for chaining
    return *this;
}

Program& Program::Link()
{
    glLinkProgram(handle);
    
    GLint r;
    glGetProgramiv(handle, GL_LINK_STATUS, &r);
    if(r == GL_FALSE)
    {
        GLchar messages[2048];
        glGetProgramInfoLog(handle, sizeof(messages), 0, &messages[0]);
        glDeleteProgram(handle);

        std::cerr << "[FATAL] [PROGRAM " << handle << "] Linking failed with error: " << messages << std::endl;
        handle = 0;
        
        throw std::runtime_error("Program linking failed");
        return *this;
    }
    
    // Log
    std::cout << "[INFO] [PROGRAM " << handle << "] Linking successful [";
    for(auto shader : shaders)
    {
        std::cout << shader->GetHandle() << ",";
    }
    std::cout << "]" << std::endl;

    // Return a reference to ourself for chaining
    linked = true;
    return *this;
}

void Program::Bind() const 
{
    glUseProgram(handle);
}

GLuint Program::GetUniform(const std::string& uniformName) const
{
    if(!IsLinked())
        return 0;
    
    auto uniformId = uniforms.find(uniformName);
    if(uniformId == uniforms.end())
    {
        auto result = uniforms.insert(std::make_pair(uniformName, glGetUniformLocation(handle, uniformName.c_str())));
        uniformId = result.first;
    }
    
    return uniformId->second;
}

GLuint Program::GetAttribute(const std::string& attributeName) const
{
    if(!IsLinked())
        return 0;
    
    auto attributeId = attributes.find(attributeName);
    if(attributeId == attributes.end())
    {
        auto result = attributes.insert(std::make_pair(attributeName, glGetAttribLocation(handle, attributeName.c_str())));
        attributeId = result.first;
    }
    
    return attributeId->second;
}
