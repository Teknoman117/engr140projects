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
};

#endif
