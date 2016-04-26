#include <iostream>
#include "Monitor.h"

int Monitor::count = 0;

void test(){
    static Monitor mon;
    mon.incident();
    mon.print();
}

int main(int argc, const char * argv[])
{
    Monitor m;

    m.incident();
    m.incident();
    
    m.print();
    
    test();
    test(); 
    return 0;
}

