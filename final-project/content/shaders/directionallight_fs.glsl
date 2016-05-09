#version 420

uniform sampler2D gBufferDiffuseSpecular;
uniform sampler2D gBufferNormals;
uniform sampler2D gBufferPosition;

uniform vec3 LightColor;
uniform vec3 LightIntensity;
uniform vec3 LightDirection;

layout (std140, binding=0) uniform CameraParameters
{
    mat4 V;
    mat4 P;
    mat4 VP;

    vec3 CameraPosition;
    
    vec4 frustumPlanes[6];
    vec4 viewport;
};

layout (location = 0) out vec4 lightingContribution;

void main ()
{
    ivec2 texel = ivec2(gl_FragCoord.xy);
    
    vec4  DiffSpecular = texelFetch(gBufferDiffuseSpecular, texel, 0);
    vec3  Normal       = texelFetch(gBufferNormals, texel, 0).rgb;
    vec3  Position     = texelFetch(gBufferPosition, texel, 0).rgb;

    vec3  AmbientColor = LightColor * LightIntensity.x;
    float DiffuseFactor = dot(Normal, -LightDirection);

    vec3  DiffuseColor = vec3(0,0,0);
    vec3  SpecularColor = vec3(0,0,0);

    if(DiffuseFactor > 0.0)
    {
        DiffuseColor = LightColor * LightIntensity.y * DiffuseFactor;

        if(Position != vec3(1.0/0.0, 1.0/0.0, 1.0/0.0))
        {
            vec3 VertexToEye = normalize(CameraPosition-Position);
            vec3 LightReflect = normalize(reflect(LightDirection, Normal));

            float SpecularFactor = dot(VertexToEye, LightReflect);
            if(SpecularFactor > 0.0)
            {
                SpecularFactor = pow(SpecularFactor, DiffSpecular.a);
                SpecularColor = LightColor * LightIntensity.z * SpecularFactor;
            }
        }
    }

    lightingContribution = vec4(DiffSpecular.rgb * (AmbientColor + DiffuseColor + SpecularColor), 1);
}

