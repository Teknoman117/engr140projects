#include <iostream>
#include <iomanip>
#include <math.h>
#include "Time.h"

using namespace std;

int main(int argc, const char * argv[])
{
    float hx, hy, mx, my, sx, sy;
    
    Time theTime = Time(4, 47, 12);
    
    theTime.getHourCoords(hx, hy);
    theTime.getMinuteCoords(mx, my);
    theTime.getSecondsCoords(sx, sy);
    
    cout << fixed << showpoint  << setprecision (3) << "hrs: (" << hx << ", " << hy << ")\n" << "min: (" << mx << ", " << my << ")\n" << "sec: (" << sx << ", " << sy << ")" << endl;
    return 0;
}

