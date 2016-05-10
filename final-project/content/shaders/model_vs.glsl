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

// The vertex coordinate
layout (location = 0) in vec3 vVert;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTex0;
layout (location = 3) in vec2 vTex1;

// Bone weights
layout (location = 4) in vec2 boneWeight0;
layout (location = 5) in vec2 boneWeight1;
layout (location = 6) in vec2 boneWeight2;
layout (location = 7) in vec2 boneWeight3;

// Is this model skinned
uniform int skinned;

// Premultiplied model view matrix
uniform mat4 M;

// The current state of bones
uniform mat4 bones[20];

// Output data to fragment stage
smooth out vec3  Pos;
smooth out vec3  Norm;
smooth out vec2  Tex;

void main()
{
    // Convert the position to a 4 dimensional vector
    vec4 position = vec4(vVert, 1.0);
    
    // Conver the normal to a 4 dimensional vector
    vec4 normal = vec4(vNorm, 0.0);
    
    // Should we apply a skinning transformation
    if(skinned > 0)
    {
        // All skinned models have at least one bone per vertex
        mat4 BoneTransform = bones[int(boneWeight0.x)] * boneWeight0.y;
        
        // Apply other bones if we have them
        if(boneWeight1.y > 0.0)
            BoneTransform += bones[int(boneWeight1.x)] * boneWeight1.y;
        if(boneWeight2.y > 0.0)
            BoneTransform += bones[int(boneWeight2.x)] * boneWeight2.y;
        if(boneWeight3.y > 0.0)
            BoneTransform += bones[int(boneWeight3.x)] * boneWeight3.y;
        
        // Calculate the new vertex position (after skinning transform)
        position = BoneTransform * position;
        
        // Calculate the new normal position (after skinning transform)
        normal = BoneTransform * normal;
    }
    
	// Output the texture coordinates to the next shader stage
	Tex = vTex0;
    
    // Per vertex attributes
    position = M * position;
    normal   = M * normal;

    Pos = position.xyz;
    Norm = normal.xyz;
    
    // Calculate the position of the vertex
    gl_Position = VP * position;
}
