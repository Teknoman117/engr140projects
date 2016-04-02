#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>
#include "Object.h"

using namespace std;


int main (int argc, const char * argv[])
{
    //Creating and initializing two Numbers and two Words
    Number n1 = Number(3);
    Number n2 = Number(5);
    Word w1 = Word("Merced");
    Word w2 = Word("UCM");
    
    //Upcasting to Object pointers
    vector<Object*> objects;
    objects.push_back(&n1); // 3
    objects.push_back(&w1); // Merced
    objects.push_back(&n2); // 5
    objects.push_back(&w2); // UCM
    
    for ( unsigned i=0; i<objects.size(); i++ ) objects[i]->print();
    
    //Performing multiplication operations. Storing results
    Object* o1 = objects[0]->multiply ( objects[2] );
    Object* o2 = objects[3]->multiply ( objects[2] );
    Object* o3 = objects[1]->multiply ( objects[3] );
    //Printing results
    o1->print();
    o2->print();
    o3->print();
    
    //multiply will return new object and we will be responsible to delete them:
    delete o1;
    delete o2;
    delete o3;
    return 0;
}

