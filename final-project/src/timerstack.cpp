#include <project/timerstack.hpp>
#include <sstream>
#include <iomanip>

GLTimerStack::GLTimerStack(size_t reserve)
{
    timers.resize(reserve);
    currentTop = -1;
}

void GLTimerStack::PushTimer(const std::string& name)
{
    //if(currentTop==-1)
        //printf("\033[H\033[J");

    currentTop++;

    if(currentTop >= timers.size())
        timers.resize(timers.size()*2 + 1);

    timers[currentTop].Start(name);
}

void GLTimerStack::PopTimer()
{
    if(currentTop == -1)
        return;

    GLuint64 elapsed = timers[currentTop].Stop();

    std::ostringstream message;
    //message << std::setw(20);
    for(int i = 0; i < currentTop; i++)
        message << "+---";

    message << timers[currentTop].name;
    
    std::cout << std::setw(30) << std::left << message.str() << ":";
    std::cout << std::setw(12) << std::left << float(elapsed) / 1000000.f << std::left << " ms";
    std::cout << std::endl;

    currentTop--;
}
