#version 420

smooth in vec3 Position;
smooth in vec3 Normal;

layout (location = 0) out vec4 gBufferDiffuseSpecular;
layout (location = 1) out vec3 gBufferNormals;
layout (location = 2) out vec3 gBufferPosition;

uniform vec3  DiffuseColor;
uniform float SpecularExponent;

void main()
{
    gBufferDiffuseSpecular = vec4(DiffuseColor, SpecularExponent);
    gBufferNormals         = Normal;
    gBufferPosition        = Position;
}