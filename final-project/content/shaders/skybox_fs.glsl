#version 420
#extension GL_NV_shadow_samplers_cube : enable

uniform samplerCube uTexture;
smooth in vec3 eyeDirection;
flat in vec3 normal;

layout (location = 0) out vec4 gBufferDiffuseSpecular;
layout (location = 1) out vec3 gBufferNormals;
layout (location = 2) out vec3 gBufferPosition;

void main ()
{
	gBufferDiffuseSpecular = vec4(texture(uTexture, eyeDirection).rgb, 0);
    gBufferNormals = vec3(0,1,0);
    gBufferPosition = vec3(1.0/0.0, 1.0/0.0, 1.0/0.0);
}

