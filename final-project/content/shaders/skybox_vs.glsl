#version 420

layout (std140, binding=0) uniform CameraParameters
{
    mat4 V;
    mat4 P;
    mat4 VP;

    vec3 CameraPosition;
    
    vec4 frustumPlanes[6];
    vec4 viewport;
};

layout(location = 0) in vec4 aPosition;

smooth out vec3 eyeDirection;

void main() 
{
    mat4 inverseProjection = inverse(P);
    mat3 inverseModelview = transpose(mat3(V));
    vec3 unprojected = (inverseProjection * aPosition).xyz;
    eyeDirection = inverseModelview * unprojected;

    gl_Position = aPosition;
} 