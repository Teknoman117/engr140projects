#ifndef __COUNTER_H__
#define __COUNTER_H__

#include <iostream>

class Counter
{
public:
    static int count;
    int ID;

public:
    Counter() : ID(count++)
    {
        std::cout << ID << " created" << std::endl;
    }
    ~Counter()
    {
        std::cout << ID << " destroyed" << std::endl;
    }
    
    void* operator new (size_t size)
    {
        std::cout << "new" << std::endl;
        return malloc(size);
    }
    
    void operator delete(void* ptr)
    {
        std::cout << "delete" << std::endl;
        free(ptr);
    }
};

#endif
