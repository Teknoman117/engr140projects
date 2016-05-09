#include <project/objects/glsphere.hpp>
#include <cmath>

using namespace glm;

namespace
{
    vec3 midpoint(const vec3& a, const vec3& b)
    {
        return (a+b)/2.0f;
    }
    
    // A class to manage the subdivision of the sphere
    struct Face
    {
        vec3 a, b, c;
        vec3 na, nb, nc;
        unsigned short subdivison;
        
        // method to generate a surface from this
        void build(std::vector<vec3>& V, std::vector<vec3>& N, float radius, unsigned short lod)
        {
            // If this level is the desired, write it out
            if(lod == subdivison)
            {
                V.push_back(a); N.push_back(na);
                V.push_back(b); N.push_back(nb);
                V.push_back(c); N.push_back(nc);
                return;
            }
            
            // Compute the edges
            vec3 nap = normalize(midpoint(a, b));
            vec3 nbp = normalize(midpoint(b, c));
            vec3 ncp = normalize(midpoint(c, a));
            vec3 ap = nap * radius;
            vec3 bp = nbp * radius;
            vec3 cp = ncp * radius;
            
            // Create the 4 subdivided faces
            Face f;
            f.subdivison = subdivison+1;
            
            // face 1 (center)
            f.a = ap; f.na = nap;
            f.b = bp; f.nb = nbp;
            f.c = cp; f.nc = ncp;
            f.build(V, N, radius, lod);
            
            // face 2 (north)
            f.a = a; f.na = na;
            f.b = ap; f.nb = nap;
            f.c = cp; f.nc = ncp;
            f.build(V, N, radius, lod);
            
            // face 3 (south east)
            f.a = ap; f.na = nap;
            f.b = b; f.nb = nb;
            f.c = bp; f.nc = nbp;
            f.build(V, N, radius, lod);
            
            // face 4 (south west)
            f.a = cp; f.na = ncp;
            f.b = bp; f.nb = nbp;
            f.c = c; f.nc = nc;
            f.build(V, N, radius, lod);
        }
    };
}

GLSphere::GLSphere( float radius, unsigned short resolution )
    : GLObject(2)
{
    std::vector<vec3> V, N;
    V.reserve(8 * pow( 4, resolution ) * 3);
    N.reserve(8 * pow( 4, resolution ) * 3);
    
    // Generate the 8 faces of the sphere
    Face f;
    f.subdivison = 0;
    
    // Face 1
    f.na = normalize(vec3(0,radius,0)); f.a = f.na * radius;
    f.nb = normalize(vec3(0,0,-radius)); f.b = f.nb * radius;
    f.nc = normalize(vec3(radius,0,0)); f.c = f.nc * radius;
    f.build(V, N, radius, resolution);
    
    // Face 2
    f.na = normalize(vec3(0,radius,0)); f.a = f.na * radius;
    f.nb = normalize(vec3(-radius,0,0)); f.b = f.nb * radius;
    f.nc = normalize(vec3(0,0,-radius)); f.c = f.nc * radius;
    f.build(V, N, radius, resolution);
    
    // Face 3
    f.na = normalize(vec3(0,radius,0)); f.a = f.na * radius;
    f.nb = normalize(vec3(0,0,radius)); f.b = f.nb * radius;
    f.nc = normalize(vec3(-radius,0,0)); f.c = f.nc * radius;
    f.build(V, N, radius, resolution);
    
    // Face 4
    f.na = normalize(vec3(0,radius,0)); f.a = f.na * radius;
    f.nb = normalize(vec3(radius,0,0)); f.b = f.nb * radius;
    f.nc = normalize(vec3(0,0,radius)); f.c = f.nc * radius;
    f.build(V, N, radius, resolution);
    
    // Face 5
    f.na = normalize(vec3(0,0,-radius)); f.a = f.na * radius;
    f.nb = normalize(vec3(0,-radius,0)); f.b = f.nb * radius;
    f.nc = normalize(vec3(radius,0,0)); f.c = f.nc * radius;
    f.build(V, N, radius, resolution);
    
    // Face 6
    f.na = normalize(vec3(-radius,0,0)); f.a = f.na * radius;
    f.nb = normalize(vec3(0,-radius,0)); f.b = f.nb * radius;
    f.nc = normalize(vec3(0,0,-radius)); f.c = f.nc * radius;
    f.build(V, N, radius, resolution);
    
    // Face 7
    f.na = normalize(vec3(0,0,radius)); f.a = f.na * radius;
    f.nb = normalize(vec3(0,-radius,0)); f.b = f.nb * radius;
    f.nc = normalize(vec3(-radius,0,0)); f.c = f.nc * radius;
    f.build(V, N, radius, resolution);
    
    // Face 8
    f.na = normalize(vec3(radius,0,0)); f.a = f.na * radius;
    f.nb = normalize(vec3(0,-radius,0)); f.b = f.nb * radius;
    f.nc = normalize(vec3(0,0,radius)); f.c = f.nc * radius;
    f.build(V, N, radius, resolution);
    
    // figure out the area of one triangle
    //_crossDistance = dist(V[1], V[2]);
    
    // send data to OpenGL buffers:
    glBindVertexArray(vertexArrayObject);
    {
        glEnableVertexAttribArray ( 0 );
        glBindBuffer ( GL_ARRAY_BUFFER, buffers[0] );
        glBufferData ( GL_ARRAY_BUFFER, V.size()*3*sizeof(float), &V[0], GL_STATIC_DRAW );
        glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

        glEnableVertexAttribArray ( 1 );
        glBindBuffer ( GL_ARRAY_BUFFER, buffers[1] );
        glBufferData ( GL_ARRAY_BUFFER, N.size()*3*sizeof(float), &N[0], GL_STATIC_DRAW );
        glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    }
    glBindVertexArray(0);
    
    // Final element count
    numPoints = V.size();
}

void GLSphere::Draw()
{
    glBindVertexArray (vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) numPoints);
}
