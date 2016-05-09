#ifndef __TIMER_STACK_HPP__
#define __TIMER_STACK_HPP__

#include <project/common.hpp>
#include <vector>

class GLTimerStack
{
    // Query class for perf monitoring
    struct GLTimer
    {
        GLuint      timers[4];
        GLuint64    startTime;
        GLuint64    endTime;

        bool        bufferSelect;
        std::string name;

        GLTimer() : bufferSelect(false)
        {
            glGenQueries(4, timers);
        }
        ~GLTimer()
        {
            glDeleteQueries(4, timers);
        }

        void Start(const std::string& name)
        {
            this->name = name;
            glQueryCounter(timers[bufferSelect ? 1 : 0], GL_TIMESTAMP);
            glGetQueryObjectui64v(timers[bufferSelect ? 0 : 1], GL_QUERY_RESULT, &startTime);
        }

        GLuint64 Stop()
        {
            this->name = name;
            glQueryCounter(timers[bufferSelect ? 3 : 2], GL_TIMESTAMP);
            glGetQueryObjectui64v(timers[bufferSelect ? 2 : 3], GL_QUERY_RESULT, &endTime);
            bufferSelect = !bufferSelect;
            return endTime - startTime;
        }
    };

    std::vector<GLTimer> timers;
    ssize_t              currentTop;

public:
    GLTimerStack(size_t reserve = 8);

    void PushTimer(const std::string& name);
    void PopTimer();
};

#endif
