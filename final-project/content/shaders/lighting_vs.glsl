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

layout(location = 0) in vec3 position;

//uniform mat4 MVP;

void main()
{
    //gl_Position = MVP * vec4(position, 1);
    gl_Position = vec4(position, 1);
}
