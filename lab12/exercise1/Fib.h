#ifndef __FIB_H__
#define __FIB_H__

class Fib
{
public:
	static int last;
	static int secondLast;

	int next(bool reset = false)
	{
		if(reset)
		{
			last = 1;
			secondLast = 0;
		}

		int n = last + secondLast;
		secondLast = last;
		last = n;
		return n;
	}
};

#endif
