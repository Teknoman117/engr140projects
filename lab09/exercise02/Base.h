#ifndef LA7_Base_h
#define LA7_Base_h

#include <iostream>

using namespace std;

class Base {
    
public:
    Base() {
        cout << "Constructing Base" << endl;
    }
    
    virtual ~Base(){
        cout << "Destroying Base" << endl;
    }
};


#endif
