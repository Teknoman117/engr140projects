/*
 *  Copyright 2016 Nathaniel Lewis
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "Material.h"

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
Material::Material(const Json::Value& value, const std::string directory)
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
            // Get a pointer to the texture type
            const std::string *it = std::find(&TextureTypeKeys[0], (&TextureTypeKeys[0]) + TextureTypeCount, (*tIt)["type"].asString());
            
            // Figure out the id
            TextureType type = (TextureType) std::distance(&TextureTypeKeys[0], it);
            
            // Add the texture
            textures[type] = directory + "/" + (*tIt)["filename"].asString();
        }
    }
    
    // Search for the diffuse color type
    const Json::Value& diffuse_color = value.get("diffuse", Json::Value::null);
    
    // Do we have a diffuse color
    if(!diffuse_color.isNull() && diffuse_color.isArray())
    {
        // Create a color storage vector and cache it
        colorDiffuse = glm::vec3(diffuse_color[0].asFloat(), diffuse_color[1].asFloat(), diffuse_color[2].asFloat());
    } else
    {
        colorDiffuse = glm::vec3(1.0, 1.0, 1.0);
    }
    
    // Search for particular color types
    const Json::Value& ambient_color = value.get("ambient", Json::Value::null);
    
    // Do we have a diffuse color
    if(!ambient_color.isNull() && ambient_color.isArray())
    {
        // Create a color storage vector and cache it
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
const vec3& Material::ColorDiffuse() const
{
    return colorDiffuse;
}

const vec3& Material::ColorAmbient() const
{
    return colorAmbient;
}

const bool& Material::Opaque() const
{
    return opaque;
}
