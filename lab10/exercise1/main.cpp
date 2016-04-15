#include "SomeHeader.h"
#include <typeinfo>

int main ()
{
	vector<A*> v;
	initVec(v);

	for(vector<A*>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << typeid(*(*it)).name() << endl;
	}

	return 0;
}
