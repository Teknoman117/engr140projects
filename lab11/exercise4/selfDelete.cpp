#include <iostream>
#include <vector>
#include "X.h"
using namespace std;


int main(int argc, char *argv[]) {
	Obj* o = new Obj("Obj1");
	vector<X*> v;
	for ( int i=0; i<10; i++ ) v.push_back(new X(o));
	for ( int i=0; i<10; i++ ) delete v[i];
	
    //At this point the object o Obj1 is deleted because it is no longer needed
    
	o = new Obj("Obj2");
	o->ref();   //This allows us to keep the object alive even after x1 and x2 are deleted
	X* x1 = new X(o);
	X* x2 = new X(o);
	delete x1;
	delete x2;
	
	cout << o->n << " is still here!" << endl;
	
	o->unref();
    
    return 0;
}

