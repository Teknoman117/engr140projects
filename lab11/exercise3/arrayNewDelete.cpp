#include <iostream>
#include "Counter.h"

int Counter::count = 0;

int main(int argc, const char * argv[])
{
    Counter * myCounter = new Counter [5];
    
    Counter * anotherCounter = new Counter [3];
    
    delete [] myCounter;
    
    delete [] anotherCounter;
    
    return 0;
}

