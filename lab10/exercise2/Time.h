#ifndef _MYTIME_H_
#define _MYTIME_H_

#include <ctime>

class Time
{ 
    int h;
    int m;
    int s;

public:
    Time()
    {
        std::time_t t = std::time(NULL);
        std::tm* tm = std::localtime ( &t ); // returns pointer to a static object
        h = tm->tm_hour;
        m = tm->tm_min;
        s = tm->tm_sec;
    }

    Time(int h, int m, int s)
    	: h(h), m(m), s(s)
    {
    }

    int getHours()
    {
    	return h;
    }

    int getMinutes()
    {
    	return m;
    }

    int getSeconds()
    {
    	return s;
    }

    void add(const Time& t)
    {
    	// seconds
    	int rs = s + t.s;
    	s = rs % 60;

    	// minutes
    	int rm = m + t.m + (rs/60);
    	m = rm % 60;

    	// hours
    	int rh = h + t.h + (rm/60);
    	h = rh % 24;
    }
};

#endif
