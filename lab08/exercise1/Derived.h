#ifndef LA6_Derived_h
#define LA6_Derived_h

#include <iostream>
#include "ADT.h"

class Derived : public ADT {
    
public:
    void doSomething()
    {
        std::cout << "I did something" << std::endl;
    }

    void doSomethingElse()
    {
        std::cout << "I did something else" << std::endl;
    }
 
    void dontDoThis()
    {
    }
};

#endif
