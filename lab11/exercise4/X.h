#ifndef LA9_X_h
#define LA9_X_h

#include "Obj.h"

class X {
public:
	Obj* o;
	
	X(Obj*pt) {
		o=pt; o->ref();
	}
	
	~X() {
		o->unref();
	}
};

#endif
