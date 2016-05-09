#ifndef __PERSISTENT_BUFFER_HPP__
#define __PERSISTENT_BUFFER_HPP__

#include <project/common.hpp>

template <typename T>
class PersistentBuffer
{
    GLuint buffer;
    GLsync locks[3];

    size_t current;
    size_t sectionSize;
    
    T*     mapping;

public:
    void CreateBuffer(size_t elements)
    {
        sectionSize = elements;
        current = 0;
        locks[0] = 0;
        locks[1] = 0;
        locks[2] = 0;

        GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT;

        glNamedBufferStorage(buffer, sizeof(T)*elements*3, 0, flags);

        mapping = static_cast<T *>(glMapNamedBufferRange(buffer, 0, sizeof(T)*elements*3, flags));
        std::cout << "[INFO] [PERSISTENT BUFFER " << buffer 
                  << "] Created with element count per slice: " << elements 
                  << ", total size = " << (sizeof(T)*elements*3) 
                  << std::endl; 
    }
    
    void BindBuffer(GLenum target)
    {
        glBindBuffer(target, buffer);
    }

    T* GetBuffer()
    {
        // Wait on the buffer to be free
        if(locks[current])
        {
            GLbitfield waitFlags = 0;
            while(1)
            {
                GLenum ret = glClientWaitSync(locks[current], waitFlags, 1);
                if(ret == GL_ALREADY_SIGNALED || ret == GL_CONDITION_SATISFIED)
                    break;
                
                if(ret == GL_WAIT_FAILED)
                {
                    std::cerr << "[WARN] [PERSISTENT BUFFER " << buffer << "] glClientWaitSync returned GL_WAIT_FAILED" << std::endl;
                    break;
                }

                waitFlags = GL_SYNC_FLUSH_COMMANDS_BIT;
                std::cerr << "[WARN] [PERSISTENT BUFFER " << buffer << "] waiting on gpu" << std::endl;
            }
        }else
        {
            std::cout << "[INFO] [PERSISTENT BUFFER " << buffer << "] skipping lock for " << current << std::endl; 
        }

        return mapping + current*sectionSize;
    }

    GLvoid* GetBufferOffset()
    {
        return (GLvoid *) (sizeof(T)*current*sectionSize);
    }

    void FlushBuffer()
    {
        glFlushMappedNamedBufferRange(buffer, current*sectionSize*sizeof(T), sectionSize*sizeof(T));

        if(locks[current])
            glDeleteSync(locks[current]);
        locks[current] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

        current = (current+1) % 3;
    }

    PersistentBuffer ()
    {
        glCreateBuffers(1, &buffer);
    }
    ~PersistentBuffer()
    {
        glUnmapNamedBuffer(buffer);
        glDeleteBuffers(1, &buffer);
    }
};

#endif
