#version 400

// Input fragment color from vertex lighting stage
in vec4 fragment_color_in;

in vec4 Pos;
in vec2 Tex;
in vec3 Norm;

// Are we textured
uniform int textured;

//Texture input
uniform sampler2D diffuse;

uniform int shadowMode;

uniform mat4 V;

// Color output
out vec4 fragmentColor;

void main ()
{

    
    if(shadowMode == 1)
    {
        fragmentColor = vec4(0,0,0,1);
    }
    else
    {
        vec4 light = V * vec4(0,3,0,1);
        
        vec3 p = Pos.xyz / Pos.w;
        vec3 lp = light.xyz / light.w;
        
        vec3 l = normalize(lp - p);
        vec3 n = normalize(Norm);
        vec4 d = fragment_color_in * dot(l, n);
        
        if(textured > 0)
        {
            d = d * texture(diffuse, Tex);
        }
        
        if(dot(l,n) < 0)
        {
            d = vec4(0,0,0,1);
        }
        
        fragmentColor = d;
    }
}

