#ifndef __OBJ_H__
#define __OBJ_H__

#include "Shared.h"
#include <iostream>

class Obj : public Shared
{
public:
    std::string n;
    
    Obj(const std::string& name)
        : n(name)
    {
        std::cout << "New " << n << std::endl;
    }
    ~Obj()
    {
        std::cout << "Delete " << n << std::endl;
    }
};

#endif
