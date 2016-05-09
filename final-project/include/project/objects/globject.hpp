#ifndef __OBJECTS_GLOBJECT_HPP__
#define __OBJECTS_GLOBJECT_HPP__

#include <vector>
#include <project/common.hpp>

class GLObject
{
protected:
	std::vector<GLuint> buffers;
	GLuint              vertexArrayObject;

public:
	GLObject(size_t bufferCount = 1);
	virtual ~GLObject();

	
};

#endif
