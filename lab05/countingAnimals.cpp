#include <iostream>
#include "Animal.h"

using namespace std;

int Animal::count = 0;

int main(int argc, const char * argv[])
{
    
    Animal myAnimal;
    Animal anotherAnimal;
    
    cout << Animal::count << endl;
    return 0;
}

