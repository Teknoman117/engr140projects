#include <project/objects/glline.hpp>

using namespace glm;

namespace 
{
    struct Vertex
    {
        vec3 position;
        vec3 normal;
    };
}

GLLine::GLLine(const std::vector<vec3>& coordinates)
    : GLObject(1)
{
    std::vector<Vertex> line;
    for(std::vector<vec3>::const_iterator coord = coordinates.begin(); coord != coordinates.end(); coord++)
    {
        line.push_back({
            .position = *coord,
            .normal = vec3(0,1,0),
        });
    }
    points = line.size();

    glBindVertexArray(vertexArrayObject);
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*line.size(), (GLvoid *) line.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, normal));
    }
    glBindVertexArray(0);
}

void GLLine::Draw()
{
    glBindVertexArray(vertexArrayObject);
    glDrawArrays(GL_LINE_STRIP, 0, points);
}
