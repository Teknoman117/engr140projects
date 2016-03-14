#ifndef LA5_Cat_h
#define LA5_Cat_h

#include "Animal.h"

class Cat : public Animal 
{
public:
    
    Cat() : Animal(){
        
    }
    
    Cat (string n, int a) : Animal(n, a){
        
    }
    
    void talk (){
        cout << "Meow, meow!" << endl;
    }
};

#endif
