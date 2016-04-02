#include <iostream>

#include "Derive.h"

int main(int argc, const char * argv[])
{
    Base * myObj = new Derive;
    
    delete myObj;
    
    return 0;
}

