#ifndef _HEADER_H_
#define _HEADER_H_

#include <iostream>
#include <algorithm>

struct AppRect
{
    int x;
    int y;
    int w;
    int h;
    
    AppRect(int x, int y, int w, int h)
        : x(x), y(y), w(w), h(h)
    {
    }
    
    AppRect(const AppRect& rect)
        : x(rect.x), y(rect.y), w(rect.w), h(rect.h)
    {
    }
};

class AppWindow
{
protected:
    AppRect frame;
    
public:
    AppWindow(int x, int y, int w, int h)
        : frame(x,y,w,h)
    {
    }
    
    AppWindow(const AppRect& frame)
        : frame(frame)
    {
    }
    
    const AppRect& rect() const
    {
        return frame;
    }
    
    virtual void resize(int w, int h) = 0;
};

class CircleWin : public AppWindow
{
public:
    CircleWin(int x, int y, int w, int h)
        : AppWindow(x,y,w,h)
    {
    }
    
    CircleWin(const AppRect& frame)
        : AppWindow(frame)
    {
    }
    
    void resize(int w, int h)
    {
        frame.w = w;
        frame.h = h;
        
        std::cout << "radius: " << std::min(frame.w, frame.h) << std::endl;
    }
};

class RectWin : public AppWindow
{
public:
    RectWin(int x, int y, int w, int h)
        : AppWindow(x,y,w,h)
    {
    }
    
    RectWin(const AppRect& frame)
        : AppWindow(frame)
    {
    }
    
    void resize(int w, int h)
    {
        frame.w = w;
        frame.h = h;
        
        std::cout << "area: " << frame.w * frame.h << std::endl;
    }
};

#endif

