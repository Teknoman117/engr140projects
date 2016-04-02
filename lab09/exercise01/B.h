#ifndef LA7_B_h
#define LA7_B_h

#include <iostream>

using namespace std;


class B : public A{
public:
    B(){
        
    }
    
    void doSomething(){
        cout << "B::doSomething" << endl;
    }
};

#endif
