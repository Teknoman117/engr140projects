#include <project/objects/globject.hpp>

GLObject::GLObject(size_t bufferCount)
{
	glGenVertexArrays(1, &vertexArrayObject);
	buffers.resize(bufferCount);
	glGenBuffers(bufferCount, buffers.data());
}

GLObject::~GLObject()
{
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(buffers.size(), buffers.data());
}
