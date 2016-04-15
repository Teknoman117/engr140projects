#include <iostream>
#include <iomanip>
#include <math.h>
#include "Time.h"

using namespace std;

int main(int argc, const char * argv[])
{
    float h = 0;
    float m = 0;
    float s = 0;
    
    Time theTime = Time(9, 30, 0);
    
    theTime.getAngles(h, m, s);
    
    cout << fixed << showpoint  << setprecision (6) << h << " : " << m << " : " << s << endl;
    
    return 0;
}

