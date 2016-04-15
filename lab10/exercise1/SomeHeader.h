#ifndef LA8_SomeHeader_h
#define LA8_SomeHeader_h
#include <iostream>
#include <vector>

using namespace std;
class A {
    
public:
    A(){
        
    }
    
    virtual void doSomething(){
        cout << "Hello" << endl;
    }
    
    ~A(){
        
    }
};

class B : public A {
    
public:
    B(){
        
    }
    
    void doSomething(){
        cout << "Hi" << endl;
    }

    ~B(){
        
    }
};


void initVec(vector<A*> & myVec){
    myVec.clear();
    myVec.push_back(new A());
    myVec.push_back(new A());
    myVec.push_back(new B());
    myVec.push_back(new A());
    myVec.push_back(new B());
    myVec.push_back(new B());
}






#endif
