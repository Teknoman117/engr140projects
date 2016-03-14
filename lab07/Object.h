#ifndef __OBJECT_H__
#define __OBJECT_H__

class Object
{
public:
    static int count;
  
    Object()
    {
        count++;
    }
    
    Object(const Object& o)
    {
        count++;
    } 
};

#endif

