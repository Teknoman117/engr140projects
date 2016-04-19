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
        //std::cout << ID << " created" << std::endl;
    }
    ~Counter()
    {
        //std::cout << ID << " destroyed" << std::endl;
    }
    
    void* operator new (size_t size)
    {
        std::cout << "new" << std::endl;
        return malloc(size);
    }
    
    void* operator new[] (size_t size)
    {
        size_t countN = (size - 4) / sizeof(Counter);
        
        std::cout << "new " << countN << " counters ";
        std::cout << "starting at " << count;
        std::cout << std::endl;
        return malloc(size);
    }
    
    void operator delete(void* ptr)
    {
        std::cout << "delete" << std::endl;
        free(ptr);
    }
        
    void operator delete[](void* ptr)
    {
        std::cout << "delete counters" << std::endl;
        free(ptr);
    }
};

#endif
