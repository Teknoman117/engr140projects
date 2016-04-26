#ifndef __MONITOR_H__
#define __MONITOR_H__

#include <iostream>

class Monitor
{
public:
	static int count;

	void incident()
	{
		count++;
	}

	void print()
	{
		std::cout << count << " incidents" << std::endl;
	}
};

#endif
