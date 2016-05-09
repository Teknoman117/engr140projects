#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <project/common.hpp>

class Texture
{
protected:
    GLuint handle;

    GLuint width;
    GLuint height;
    GLuint levels;
    GLint  format;

public:
    Texture();
    Texture(GLuint width, GLuint height, GLuint levels = 1, GLenum format = GL_RGBA8);
    Texture(GLuint width, GLuint height, GLenum internalFormat, GLenum format, GLenum type, const GLvoid* data);
    Texture(const std::string& path, GLint format, bool mipmapped = true);
    virtual ~Texture();

    virtual void Bind() const;
    virtual void Bind(GLenum unit) const;
    virtual void SetWrapMode(GLenum mode);

    GLuint GetHandle() const;
    GLuint GetWidth()  const;
    GLuint GetHeight() const;
    GLenum GetFormat() const;
};

#endif
