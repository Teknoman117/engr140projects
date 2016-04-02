#include <iostream>
#include "SortableCircleData.h"

int main(int argc, const char * argv[])
{
    Data * myData = new Data();
    
    myData->add(new Circle());
    myData->add(new Circle(3));
    myData->add(new Circle(2));
    myData->add(new Circle(4));
    myData->add(new Circle(1));
    
    myData->print();
    
    myData->remove(3);
    
    myData->print();
    
    myData->remove(0);
    myData->remove(0);
    myData->remove(0);
    myData->remove(0);
    
    myData->print();
    
    return 0;
}

