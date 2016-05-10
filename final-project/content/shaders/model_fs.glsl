#version 420

smooth in vec3 Pos;
smooth in vec3 Norm;
smooth in vec2 Tex;

// G-Buffer outputs
layout (location = 0) out vec4 gBufferDiffuseSpecular;
layout (location = 1) out vec3 gBufferNormals;
layout (location = 2) out vec3 gBufferPosition;

uniform sampler2D diffuse;

uniform vec3  DiffuseColor;
uniform float SpecularExponent;
uniform int   textured;

void main ()
{
    gBufferDiffuseSpecular = vec4(DiffuseColor * texture(diffuse, Tex).rgb, SpecularExponent);
    gBufferNormals         = Norm;
    gBufferPosition        = Pos;
}

