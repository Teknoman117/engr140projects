#include <project/objects/glcube.hpp>

namespace
{
    // The vertices in a cube
    const GLfloat cube_vertices[] =
    {
        -1.0,  1.0,  1.0,
        -1.0, -1.0,  1.0,
        1.0, -1.0,  1.0,
        1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0,
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
        1.0,  1.0, -1.0,
    };

    // The indices in a cube
    const GLushort cube_indices[] =
    {
        0, 1, 2,
        2, 3, 0,
        3, 2, 6,
        6, 7, 3,
        7, 6, 5,
        5, 4, 7,
        4, 5, 1,
        1, 0, 4,
        0, 3, 7,
        7, 4, 0,
        1, 2, 6,
        6, 5, 1
    };
}

GLCube::GLCube()
    : GLObject(2)
{
    // Generate the storage for the vertex data of the skybox
    glBindVertexArray(vertexArrayObject);
    {
        // Upload the vertex data
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        // Upload the index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
}

GLCube::~GLCube()
{
}

void GLCube::Draw()
{
    glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, sizeof(cube_indices) / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}
