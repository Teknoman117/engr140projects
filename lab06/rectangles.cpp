#include <iostream>
#include "Rect.h"

int main(int argc, const char * argv[])
{

    Rect a(-5.0f,-5.0f,2.0f,2.0f), b(5.0f,8.0f,2.0f,2.0f);
    if ( a.contains(Vec::null) ) std::cout<<"error1\n";
    if ( b.contains(Vec::null) ) std::cout<<"error1\n";
    if ( !a.contains(Vec(-4.0f,-6.0f)) ) std::cout<<"error\n";
    if ( b.contains(Vec(6.0f,9.0f)) ) std::cout<<"error\n";
    std::cout << "Done\n";
    return 0;
}

