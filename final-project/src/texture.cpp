#include <project/texture.hpp>
#include <project/lodepng.hpp>

#include <vector>

namespace
{
    // lovingly borrowed from http://www.geeksforgeeks.org/write-one-line-c-function-to-find-whether-a-no-is-power-of-two/
    template <typename T>
    static bool isPowerOfTwo (T x) 
    {
        /* First x in the below expression is for the case when x is 0 */
        return x && (!(x&(x-1)));
    }
}

Texture::Texture()
{
    glGenTextures(1, &handle);
}

Texture::Texture(GLuint width, GLuint height, GLuint levels, GLenum format)
    : Texture()
{
    this->width = width;
    this->height = height;
    this->levels = levels;
    this->format = format;

    glBindTexture(GL_TEXTURE_2D, handle);

    glTexStorage2D(GL_TEXTURE_2D, levels, format, width, height);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Texture::Texture(GLuint width, GLuint height, GLenum internalFormat, GLenum format, GLenum type, const GLvoid* data)
    : Texture()
{
    this->width = width;
    this->height = height;
    this->levels = 1;
    this->format = format;

    glBindTexture(GL_TEXTURE_2D, handle);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Texture::Texture(const std::string& path, GLint format, bool mipmapped)
    : Texture()
{
    glBindTexture(GL_TEXTURE_2D, handle);

    // Determine the lodepng format
    LodePNGColorType colorFormat;
    unsigned         colorDepth;
    GLenum           textureFormat;

    switch(format)
    {
        // 8 bit types
        case GL_RGBA8:
        case GL_SRGB8_ALPHA8:
            colorFormat = LCT_RGBA;
            textureFormat = GL_RGBA;
            colorDepth = 8;
            break;
        case GL_RGB8:
        case GL_SRGB8:
            colorFormat = LCT_RGB;
            textureFormat = GL_RGB;
            colorDepth = 8;
            break;
        case GL_RG8:
            colorFormat = LCT_GREY_ALPHA;
            textureFormat = GL_RG;
            colorDepth = 8;
            break;
        case GL_R8:
            colorFormat = LCT_GREY;
            textureFormat = GL_RED;
            colorDepth = 8;
            break;

        // 16 bit types
        case GL_RGBA16:
            colorFormat = LCT_RGBA;
            textureFormat = GL_RGBA;
            colorDepth = 16;
            break;
        case GL_RGB16:
            colorFormat = LCT_RGB;
            textureFormat = GL_RGB;
            colorDepth = 16;
            break;
        case GL_RG16:
            colorFormat = LCT_GREY_ALPHA;
            textureFormat = GL_RG;
            colorDepth = 16;
            break;
        case GL_R16:
            colorFormat = LCT_GREY;
            textureFormat = GL_RED;
            colorDepth = 16;
            break;

        default:
            std::cerr << "[FATAL] [TEXTURE \"" << path << "\"] Unknown OpenGL => LodePNG type conversion" << std::endl;
            throw std::runtime_error("Unsupported texture format");
    }

    // Load the PNG
    std::vector<unsigned char> storage;
    unsigned w, h;
    unsigned error = lodepng::decode(storage, w, h, path, colorFormat, colorDepth);
    if(error)
    {
        std::cerr << "[FATAL] [TEXTURE \"" << path << "\"] Failed to load file with error: " << error << std::endl;
        throw std::runtime_error("Texture load failed");
    }

    // Upload the texture data
    this->width = w;
    this->height = h;
    this->format = format;
    glTexImage2D( GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, (colorDepth == 16 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE), storage.data() );

    // Default behavior is to clamp
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Generate mipmaps if supported
    std::cout << "[INFO] [TEXTURE \"" << path << "\"] Loaded (" << width << "," << height << ") Texture, ";
    if(mipmapped && isPowerOfTwo(width) && isPowerOfTwo(height))
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glGenerateMipmap(GL_TEXTURE_2D);

        // use anisotropic filtering
        GLfloat fLargest;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
        std::cout << "Filtering = " << fLargest << "xAF" << std::endl;
    }
    else
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        std::cout << "Filtering = Bilinear" << std::endl;
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &handle);
    std::cout << "[INFO] [TEXTURE " << handle << "] Destroyed" << std::endl;
}

void Texture::Bind() const 
{
    glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::Bind(GLenum unit) const
{
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::SetWrapMode(GLenum mode)
{
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
}

GLuint Texture::GetHandle() const
{
    return handle;
}

GLuint Texture::GetWidth()  const
{
    return width;
}

GLuint Texture::GetHeight() const
{
    return height;
}

GLenum Texture::GetFormat() const
{
    return format;
}

