#include <project/objects/glfullscreenquad.hpp>

using glm::vec3;

GLFullscreenQuad::GLFullscreenQuad()
	: GLObject(1)
{
	// Coordinates for a fullscreen quad in TRIANGLE_STRIPS
	const vec3 vertices[]
	{
		vec3(-1.f,  1.f, 0.f),
		vec3(-1.f, -1.f, 0.f),
		vec3( 1.f,  1.f, 0.f),
		vec3( 1.f, -1.f, 0.f),
	};

	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);

	glBindVertexArray(0);
}

void GLFullscreenQuad::Draw()
{
	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

