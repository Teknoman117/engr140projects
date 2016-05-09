#version 430

layout (rgba16f, binding = 1) writeonly uniform image2D destinationImage; 

uniform sampler2D sourceImage;
uniform int       sourceLevel;

layout (local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
    ivec2 destinationPixel = ivec2(gl_GlobalInvocationID.xy);
    ivec2 destinationImageSize = imageSize(destinationImage);

    // only perform work if the destination pixel is valid
    if(destinationPixel.x < destinationImageSize.x && destinationPixel.y < destinationImageSize.y)
    {
        // Compute xy pixel boundaries (for some reason it would fail as 1/textureSize)
        vec2 pixelInterval = textureSize(sourceImage, sourceLevel);
        pixelInterval = 1 / pixelInterval;

        // Perform sampling
        vec2 baseCoord = (2*pixelInterval) + (vec2(destinationPixel)*4*pixelInterval);

        vec4 a = textureLod(sourceImage, baseCoord - pixelInterval, sourceLevel);
        vec4 b = textureLod(sourceImage, baseCoord + vec2( pixelInterval.x, -pixelInterval.y), sourceLevel);
        vec4 c = textureLod(sourceImage, baseCoord + vec2(-pixelInterval.x,  pixelInterval.y), sourceLevel);
        vec4 d = textureLod(sourceImage, baseCoord + pixelInterval, sourceLevel);

        vec4 average = 0.25 * (a + b + c + d);

        // Store
        imageStore(destinationImage, destinationPixel, average);
    }
}
