#include <project/material.hpp>
#include <algorithm>

//#define __MATERIAL_PRINT_LOGS__

// Unique texture type count
const static unsigned int TextureTypeCount = 4;

// Since deserialization is a feature, we need to have the texture keys
const static std::string TextureTypeKeys[] =
{
    "DIFFUSE",
    "SPECULAR",
    "BUMP",
    "NORMAL",
};

// Standard constructor - create an empty material
Material::Material()
    : id(""), textures(Material::texture_store()), colorDiffuse(glm::vec3(1.0, 1.0, 1.0)), colorAmbient(glm::vec3(1.0, 1.0, 1.0)), opaque(true)
{
    
}

// Deserialization constructor - load from a Json serialized blob
Material::Material(const Json::Value& value)
    : id(""), textures(Material::texture_store()), opaque(true)
{
    // We first need to validate that this a Json object
    if(!value.isObject())
    {
        throw std::runtime_error("Material::Material(const Json::Value& value) - value must be a object");
    }
    
    // Load the name of the material
    id = value["id"].asString();
    
    // Get the potential textures
    const Json::Value& bitmaps = value["textures"];
    
    // If this mesh contains a vertices entry (there could be some empty cases, for instance, an empty mesh was created, then serialized)
    if(!bitmaps.isNull() && bitmaps.isArray())
    {
        // Read in all the textures
        for(Json::Value::const_iterator tIt = bitmaps.begin(); tIt != bitmaps.end(); tIt++)
        {
            const std::string *it = std::find(&TextureTypeKeys[0], (&TextureTypeKeys[0]) + TextureTypeCount, (*tIt)["type"].asString());
            TextureType type = (TextureType) std::distance(&TextureTypeKeys[0], it);
            textures[type] = (*tIt)["filename"].asString();
            std::cout << "found: " << textures[type] << std::endl;
        }
    }
    
    // Search for the diffuse color type
    const Json::Value& diffuse_color = value.get("diffuse", Json::Value::null);
    if(!diffuse_color.isNull() && diffuse_color.isArray())
    {
        colorDiffuse = glm::vec3(diffuse_color[0].asFloat(), diffuse_color[1].asFloat(), diffuse_color[2].asFloat());
    } else
    {
        colorDiffuse = glm::vec3(1.0, 1.0, 1.0);
    }
    
    // Search for particular color types
    const Json::Value& ambient_color = value.get("ambient", Json::Value::null);
    if(!ambient_color.isNull() && ambient_color.isArray())
    {
        colorAmbient = glm::vec3(ambient_color[0].asFloat(), ambient_color[1].asFloat(), ambient_color[2].asFloat());
    } else
    {
        colorAmbient = colorDiffuse;
    }
    
    // Search for opaque/transparent settings
    Json::Value opaque_default = true;
    opaque = value.get("opaque", opaque_default).asBool();
    
#if (defined __MATERIAL_PRINT_LOGS__)
    cout << "Loaded Material (" << id << "): ";
    cout << "Ambient => {" << colorAmbient.r << " " << colorAmbient.g << " " << colorAmbient.b << "} ";
    cout << "Diffuse => {"  << colorDiffuse.r << " " << colorDiffuse.g << " " << colorDiffuse.b << "} ";
    cout << "Textures (" << textures.size() << ")" << endl;
#endif
}

// Get the name of the material
std::string& Material::Id()
{
    return id;
}

// Get a const reference to the texture storage
const Material::texture_store& Material::Textures() const
{
    return textures;
}

// Get the material colors
const glm::vec3& Material::ColorDiffuse() const
{
    return colorDiffuse;
}

const glm::vec3& Material::ColorAmbient() const
{
    return colorAmbient;
}

const bool& Material::Opaque() const
{
    return opaque;
}
