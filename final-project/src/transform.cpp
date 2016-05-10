/*
 *  Copyright 2014 Nathaniel Lewis
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

#include <project/transform.hpp>

using namespace glm;

// Standard constructor
Transform::Transform ()
    : translation(vec3(0, 0, 0)), rotation(quat(1.0, 0.0, 0.0, 0.0)), scale(vec3(1.0, 1.0, 1.0)), dirty(true)
{
    TransformMatrix();
}

// Copy another transformation
Transform::Transform (const Transform& transform)
    : translation(transform.translation), rotation(transform.rotation), scale(transform.scale), dirty(true)
{
    TransformMatrix();
}

// Construct the transformation from a Json blob
Transform::Transform (const Json::Value& value)
    : translation(vec3(0, 0, 0)), rotation(quat(1.0, 0.0, 0.0, 0.0)), scale(vec3(1.0, 1.0, 1.0)), dirty(true)
{
    // If a translation entry exists in the node
    if(value["translation"] != Json::Value::null)
    {
        // Load the translation from the Json blob
        translation = vec3(value["translation"][0].asFloat(), value["translation"][1].asFloat(), value["translation"][2].asFloat());
    }
    
    // If a scale entry exists in the node
    if(value["scale"] != Json::Value::null)
    {
        // Load the scale transform
        scale = vec3(value["scale"][0].asFloat(), value["scale"][1].asFloat(), value["scale"][2].asFloat());
    }
    
    // If a rotation entry exists in the node
    if(value["rotation"] != Json::Value::null)
    {
        rotation = quat(value["rotation"][3].asFloat(), value["rotation"][0].asFloat(), value["rotation"][1].asFloat(), value["rotation"][2].asFloat());
    }
    
    // Calculate the transform
    TransformMatrix();
}

// Assignment operator (copy the contents of the other transform)
Transform& Transform::operator=(const Transform &transform)
{
    // Copy the contents
    translation = transform.translation;
    rotation    = transform.rotation;
    scale       = transform.scale;
    
    // Contents are now dirty
    dirty = true;
    
    // Calculate the transform
    TransformMatrix();
    
    // Return a reference to ourself
    return *this;
}

void Transform::SetTransformMatrix(mat4 transformMatrix)
{
    // Extract the translation
    translation = vec3(transformMatrix[3]);
    
    mat3 rotation = mat3(transformMatrix);
    
    // Extract the scale
    scale = vec3(length(rotation[0]), length(rotation[1]), length(rotation[2]));
    rotation[0] /= length(rotation[0]);
    rotation[1] /= length(rotation[1]);
    rotation[2] /= length(rotation[2]);
    
    // Extract the rotation
    this->rotation = toQuat(rotation);
    
    dirty = true;
}

// Return read only references to our internal content (does NOT flag content as dirty)
const vec3& Transform::Translation() const
{
    return translation;
}

const vec3& Transform::Scale() const
{
    return scale;
}

const quat& Transform::Rotation() const
{
    return rotation;
}

// Return read/write references to our internal content (flags contents as dirty)
vec3& Transform::Translation()
{
    dirty = true;
    return translation;
}

vec3& Transform::Scale()
{
    dirty = true;
    return scale;
}

quat& Transform::Rotation()
{
    dirty = true;
    return rotation;
}

// Return the transforms combined
const mat4& Transform::TransformMatrix()
{
    // If the transformation is dirty update it
    if(dirty)
    {
        // Update the transformation matrix
        transformMatrix = translate(translation) * mat4_cast(rotation) * glm::scale(scale);
        
        // Clear the dirty flag
        dirty = false;
    }
    
    // Return the transformation matrix
    return transformMatrix;
}

// Return the transforms combined
const mat4& Transform::TransformMatrix() const
{
    // If the transformation is dirty update it
    if(dirty)
    {
        throw new std::runtime_error("const mat4& Transform::TransformMatrix() const - attempted to read a dirty transformation matrix");
    }
    
    // Return the transformation matrix
    return transformMatrix;
}

// Interpolate between two transforms
Transform Transform::Interpolate(const Transform& a, const Transform&  b, const float t)
{
    // Final transform
    Transform f;
    
    // Compute the interpolated transformation
    f.Translation() = mix(a.Translation(), b.Translation(), t);
    f.Scale() = mix(a.Scale(), b.Scale(), t);
    f.Rotation() = slerp(a.Rotation(), b.Rotation(), t);
    
    // Return the interpolated transform
    return f;
}
