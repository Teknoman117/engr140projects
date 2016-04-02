#include <iostream>
#include "A.h"
#include "B.h"

using namespace std;

int main(int argc, const char * argv[])
{
    A * myObj = new B();
    myObj->start();
    return 0;
}

