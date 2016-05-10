#version 400

layout (location = 0) in vec3 vVert;
layout (location = 1) in vec2 vTex;
layout (location = 2) in vec3 vNorm;

uniform mat4 MV;
uniform mat4 P;

out vec4 Pos;
out vec2 Tex;
out vec3 Norm;

void main()
{
    // Per vertex attributes
    Pos = MV * vec4(vVert, 1.0);
    Norm = vec3(MV * vec4(vNorm, 0.0));
    Tex = vTex;
    
    // Calculate the position of the vertex
    gl_Position = P * Pos;
}
