#include <iostream>
#include "Derived.h"

using namespace std;

int main(int argc, const char * argv[])
{
    
    ADT * someVar = new Derived;
    
    someVar->doSomething();
    
    someVar->doSomethingElse();
    
    return 0;
}

