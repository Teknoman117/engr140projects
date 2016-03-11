#include <iostream>
#include "Vec.h"

using namespace std;

inline bool equal ( const Vec & u, const Vec& v ){
    return u.x==v.x && u.y==v.y;
}

int main(int argc, const char * argv[])
{

    Vec n, u(1.0f,2.0f);
    if ( !equal(n,Vec::null) ) std::cout<<"error\n";
    for ( int i=0; i<10; i++ ) n.add ( u );
    n.print();
    if ( !equal(n,Vec(10.0f,20.0f)) ) std::cout<<"error\n";
    return 0;
}

