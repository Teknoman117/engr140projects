#version 400

in vec4 Pos;
in vec2 Tex;
in vec3 Norm;

uniform mat4 V;

uniform sampler2D diffuse;

out vec4 fragmentColor;

void main ()
{
    vec4 light = V * vec4(0,3,0,1);
    
    vec3 p = Pos.xyz / Pos.w;
    vec3 lp = light.xyz / light.w;
    
    vec3 l = normalize(lp - p);
    vec3 n = normalize(Norm);
    vec4 d = texture(diffuse, Tex) * dot(l, n);
    
    if(dot(l,n) < 0)
    {
        d = vec4(0,0,0,1);
    }
    
    fragmentColor = vec4(0.1,0.1,0.1,1) + d;
}