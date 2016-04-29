#include <iostream>
#include "Vec.h"

int main(int argc, const char * argv[])
{

    Vec a = Vec(2,3) + Vec(2,2) * 4 - Vec(1,1);
    
    cout << a;
    return 0;
}

