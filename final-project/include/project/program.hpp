#ifndef __program_hpp__
#define __program_hpp__

#include <vector>
#include <map>

#include <project/common.hpp>
#include <project/shader.hpp>

class Program
{
    GLuint handle;
    bool   linked;
    
    std::vector<std::shared_ptr<Shader>> shaders;
    
    mutable std::map<std::string, GLuint> uniforms;
    mutable std::map<std::string, GLuint> attributes;
    
public:
    Program();
    virtual ~Program();
    
    bool IsValid() const;
    bool IsLinked() const;
    
    Program& Attach(const std::string& path, const GLenum type);
    Program& Attach(const std::shared_ptr<Shader>& shader);
    Program& Link();
    void     Bind() const;
    
    GLuint   GetUniform(const std::string& uniformName) const;
    GLuint   GetAttribute(const std::string& attributeName) const;
};

#endif
