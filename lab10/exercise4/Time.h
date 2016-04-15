#ifndef _MYTIME_H_
#define _MYTIME_H_

#include <ctime>
#include <cmath>

class Time
{ 
    int h;
    int m;
    int s;

    // fixed angle on the range -pi to pi
    float fixangle(float a)
    {
        while(a < -M_PI)
            a += 2.f * M_PI;
        while(a > M_PI)
            a -= 2.f * M_PI;
        return a;
    }

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

    void getAngles ( float& a, float& b, float& c )
    {
        // fix the hours
        int hp = h % 12;

        // minutes and seconds
        c = fixangle(M_PI_2 - (M_PI * (static_cast<float>(s)/30.f)));
        b = fixangle(M_PI_2 - (M_PI * (static_cast<float>(m)/30.f)));

        // hours
        a = 0.f;
        a += (M_PI * (static_cast<float>(hp)/6.f));
        a += ((M_PI/6.f) * (static_cast<float>(m)/60.f));
        a = fixangle(M_PI_2 - a);
    }

    void getHourCoords ( float& x, float& y )
    {
        float h_, m_, s_;
        getAngles(h_, m_, s_);

        x = cos(h_);
        y = sin(h_);
    }

    void getMinuteCoords ( float& x, float& y )
    {
        float h_, m_, s_;
        getAngles(h_, m_, s_);

        x = cos(m_);
        y = sin(m_);
    }

    void getSecondsCoords ( float& x, float& y )
    {
        float h_, m_, s_;
        getAngles(h_, m_, s_);

        x = cos(s_);
        y = sin(s_);
    }
};

#endif
