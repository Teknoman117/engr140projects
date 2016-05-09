#ifndef __CUBEMAP_HPP__
#define __CUBEMAP_HPP__

#include <project/common.hpp>
#include <project/texture.hpp>

class Cubemap : public Texture
{
public:
    Cubemap(const std::string& path);
    ~Cubemap();

    void Bind() const;
    void Bind(GLenum unit) const;
    void SetWrapMode(GLenum mode);
};

#endif
