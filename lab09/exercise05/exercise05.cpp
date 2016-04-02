#include <iostream>
#include "SortableCircleData.h"

using namespace std;

int main ()
{
    Data *d = new Data;
    
    bool run = true;
    char command;
    int value;
    
    while(run)
    {
        cin >> command;
        switch (command)
        {
            case 'A':
                cin >> value;
                d->add(new Circle(value));
                break;
                
            case 'R':
                cin >> value;
                d->remove(value);
                break;
                
            case 'S':
                d->sort();
                break;
                
            case 'P':
                d->print();
                break;
                
            case 'D':
                delete d;
                d = new Data;
                break;
                
            case 'Q':
                run = false;
                break;
                
            default:
                break;
        }
    }
    
    return 0;
}
