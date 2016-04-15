#include <iostream>
#include "Time.h"

using namespace std;

int main(int argc, const char * argv[])
{
    Time myTime = Time(10, 46, 32);
    Time anotherTime = Time(2, 25, 50);
    
    myTime.add(anotherTime);
    
    cout << myTime.getHours() << " : " << myTime.getMinutes() << " : " << myTime.getSeconds() << endl;
    return 0;
}

