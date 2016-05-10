#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include <project/common.hpp>
#include <json/json.h>

// The mesh is an object which
class Material
{
public:
    // Types of material textures
    typedef enum : unsigned char
    {
        kTextureTypeDiffuse  = 0,
        kTextureTypeSpecular = 1,
        kTextureTypeBump     = 2,
        kTextureTypeNormal   = 3,
    } TextureType;
    
    // Types of color attributes
    typedef enum : unsigned char
    {
        kColorAttributeAmbient    = 0,
        kColorAttributeDiffuse    = 1,
        kColorAttributeSpecular   = 2,
        kColorAttributeEmissive   = 3,
        kColorAttributeReflection = 4,
    } ColorAttribute;
    
    // Texture storage type
    typedef std::map<TextureType, std::string> texture_store;
    
    // Iterator type
    typedef std::map<ColorAttribute, glm::vec4> color_store;
    
private:
    // Name of the material
    std::string id;
    
    // The textures this material uses
    texture_store textures;
    
    // Material colors
    glm::vec3 colorDiffuse;
    glm::vec3 colorAmbient;
    
    // Is this material opaque
    bool opaque;
    
public:
    // Standard constructor - create an empty material
    Material();
    
    // Deserialization constructor - load from a Json serialized blob
    Material(const Json::Value& value);
    
    // Get the name of the material
    std::string& Id();
    
    // Get a const reference to the texture storage
    const texture_store& Textures() const;
    
    // Get the material colors
    const glm::vec3& ColorDiffuse() const;
    const glm::vec3& ColorAmbient() const;
    
    // Get if the material is opaque
    const bool& Opaque() const;
};

#endif
