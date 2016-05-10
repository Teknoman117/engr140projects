#version 400

// The vertex coordinate
in vec3 vVert;
in vec2 vTex0;
in vec2 vTex1;
in vec3 vNorm;

// Bone weights
in vec2 boneWeight0;
in vec2 boneWeight1;
in vec2 boneWeight2;
in vec2 boneWeight3;

// The current state of bones
uniform mat4 bones[20];

// Is this model skinned
uniform int skinned;

// Premultiplied model view matrix
uniform mat4 MV;

// Projection matrix
uniform mat4 P;

// Material color components
uniform vec3 material_color_ambient;
uniform vec3 material_color_diffuse;

// Output data to fragment stage
out vec4  Pos;
out vec3  Norm;
out vec2  Tex;

out vec4  fragment_color_in;

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
    
    // Make sure its opaque
    fragment_color_in = vec4(material_color_diffuse, 1.0);
    
	// Output the texture coordinates to the next shader stage
	Tex = vTex0;
    
    // Per vertex attributes
    Pos = MV * position;
    Norm = vec3(MV * normal);
    
    // Calculate the position of the vertex
    gl_Position = P * Pos;
}
