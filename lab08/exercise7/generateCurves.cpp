#include <iostream>
#include "GenerateCurves.h"

using namespace std;

int main(int argc, const char * argv[])
{
    XYData* d2 = new QuadraticCurve ( 1, 0, 0 );
    XYData* d3 = new CubicCurve ( 1, 0, 0, 0 );
    d2->generate ( 0, 2, 1 );
    d3->generate ( 0, 2, 1 );
    cout<<"Q Max: "<<d2->max().y<<endl;
    cout<<"C Max: "<<d3->max().y<<endl;

    return 0;
}

