#ifndef LA7_Derive_h
#define LA7_Derive_h

#include <iostream>
#include "Base.h"

class Derive : public Base{
    
public:
    Derive(){
        cout << "Constructing Derive" << endl;
    }
    
    ~Derive(){
        cout << "Destroying Derive" << endl;
    }
};

#endif
