#include <iostream>
#include "Cat.h"
#include "Dog.h"

int main(int argc, const char * argv[])
{
    Animal * someAnimal;
    
    someAnimal = new Cat();
    
    someAnimal->talk();
    
    delete someAnimal;
    
    someAnimal = new Dog();
    
    someAnimal->talk();
    
    delete someAnimal;
    
    return 0;
}

