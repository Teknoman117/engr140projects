#version 430

layout (rgba16f, binding = 1) writeonly uniform image2D destinationImage; 

uniform sampler2D sourceImage;
uniform int       sourceLevel;

// shared memory for the reduction
shared vec4  average[1024];
shared float contribution[1024];

layout (local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
    // preserve the destination pixel and image size as originally existed
    vec2 destinationPixel = vec2(gl_GlobalInvocationID.xy);
    vec2 destinationImageSize = textureSize(sourceImage, sourceLevel-2);

    // only perform work if the destination pixel is valid
    if(destinationPixel.x < destinationImageSize.x && destinationPixel.y < destinationImageSize.y)
    {
        // Compute xy pixel boundaries (for some reason it would fail as 1/textureSize)
        vec2 pixelInterval = textureSize(sourceImage, sourceLevel);
        pixelInterval = 1 / pixelInterval;

        // Perform sampling
        vec2 baseCoord = (2*pixelInterval) + (destinationPixel*4*pixelInterval);

        vec4 a = textureLod(sourceImage, baseCoord - pixelInterval, sourceLevel);
        vec4 b = textureLod(sourceImage, baseCoord + vec2( pixelInterval.x, -pixelInterval.y), sourceLevel);
        vec4 c = textureLod(sourceImage, baseCoord + vec2(-pixelInterval.x,  pixelInterval.y), sourceLevel);
        vec4 d = textureLod(sourceImage, baseCoord + pixelInterval, sourceLevel);

        // store into shared memory
        average[gl_LocalInvocationIndex] = 0.25 * (a + b + c + d);
        contribution[gl_LocalInvocationIndex] = 1;
    }

    // otherwise store zero (and no contribution)
    else
    {
        average[gl_LocalInvocationIndex] = vec4(0,0,0,0);
        contribution[gl_LocalInvocationIndex] = 0;
    }
    barrier();

    // Perform a parallel reduction of all the elements
    if(gl_LocalInvocationIndex < 512)
    {
        average[gl_LocalInvocationIndex] += average[gl_LocalInvocationIndex+512];
        contribution[gl_LocalInvocationIndex] += contribution[gl_LocalInvocationIndex+512];
    }
    barrier();

    if(gl_LocalInvocationIndex < 256)
    {
        average[gl_LocalInvocationIndex] += average[gl_LocalInvocationIndex+256];
        contribution[gl_LocalInvocationIndex] += contribution[gl_LocalInvocationIndex+256];
    }
    barrier();

    if(gl_LocalInvocationIndex < 128)
    {
        average[gl_LocalInvocationIndex] += average[gl_LocalInvocationIndex+128];
        contribution[gl_LocalInvocationIndex] += contribution[gl_LocalInvocationIndex+128];
    }
    barrier();

    if(gl_LocalInvocationIndex < 64)
    {
        average[gl_LocalInvocationIndex] += average[gl_LocalInvocationIndex+64];
        contribution[gl_LocalInvocationIndex] += contribution[gl_LocalInvocationIndex+64];
    }
    barrier();

    // last warp
    if(gl_LocalInvocationIndex < 32)
    {
        average[gl_LocalInvocationIndex] += average[gl_LocalInvocationIndex+32];
        contribution[gl_LocalInvocationIndex] += contribution[gl_LocalInvocationIndex+32];

        average[gl_LocalInvocationIndex] += average[gl_LocalInvocationIndex+16];
        contribution[gl_LocalInvocationIndex] += contribution[gl_LocalInvocationIndex+16];
        
        average[gl_LocalInvocationIndex] += average[gl_LocalInvocationIndex+8];
        contribution[gl_LocalInvocationIndex] += contribution[gl_LocalInvocationIndex+8];
        
        average[gl_LocalInvocationIndex] += average[gl_LocalInvocationIndex+4];
        contribution[gl_LocalInvocationIndex] += contribution[gl_LocalInvocationIndex+4];
        
        average[gl_LocalInvocationIndex] += average[gl_LocalInvocationIndex+2];
        contribution[gl_LocalInvocationIndex] += contribution[gl_LocalInvocationIndex+2];

        average[gl_LocalInvocationIndex] += average[gl_LocalInvocationIndex+1];
        contribution[gl_LocalInvocationIndex] += contribution[gl_LocalInvocationIndex+1];

        if(gl_LocalInvocationIndex==0)
        {
            vec4 avg = average[gl_LocalInvocationIndex] / contribution[gl_LocalInvocationIndex];
            imageStore(destinationImage, ivec2(gl_WorkGroupID.xy), avg);
        }
    }
}
