#ifndef __CAMERA_PARAMETERS_HPP__
#define __CAMERA_PARAMETERS_HPP__

#include <project/common.hpp>

struct CameraParameters
{
    struct ShaderParameters
    {
        glm::mat4 V;
        glm::mat4 P;
        glm::mat4 VP;

        glm::vec3 CameraPosition;
        float     padding;
        
        glm::vec4 frustumPlanes[6];
        glm::vec4 viewport;
    } shaderParameters;

    GLuint    buffer;

    GLint     width;
    GLint     height;
    float     aspect;
    float     yFov;

    glm::vec3 up;
    glm::vec3 position;
    glm::vec2 rotation;
    glm::vec4 viewport;

    CameraParameters();
    ~CameraParameters();

    glm::quat RotationAsQuaternion() const;
    void      Update();
};

#endif
