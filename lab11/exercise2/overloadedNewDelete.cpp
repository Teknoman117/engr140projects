#include <iostream>
#include <vector>
#include "Counter.h"

using namespace std;

int Counter::count = 0;

int main(int argc, const char * argv[])
{
    
    vector<Counter*> counters;
    
    for (unsigned i = 0; i < 5; i++) {
        counters.push_back(new Counter());
    }
    
    for (unsigned i = 0; i < 5; i++) {
        cout << counters[i]->ID << endl;
    }
    
    for (unsigned i = 0; i < 5; i++) {
        delete counters[i];
    }
    
    return 0;
}

