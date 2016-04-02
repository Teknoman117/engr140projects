#ifndef LA6_Sortable_h
#define LA6_Sortable_h

class Sortable {
    
public:
    virtual bool compare (const Sortable * s) = 0;
    virtual void print () = 0;
};

#endif
#ifndef __DATA__
#define __DATA__

#include <iostream>
#include <vector>
#include <algorithm>

class Data
{
    std::vector<Sortable *> data;

public:
    void add (Sortable *n)
    {
        data.push_back(n);
    }
    
    void remove(int index)
    {
        if(data.size() == 0)
        {
            std::cout << "Dataset empty" << std::endl;
            return;
        }
        
        data.erase(data.begin() + index);
    }

    void print(std::vector<Sortable *>& s)
    {
        if(s.size() == 0)
        {
            std::cout << "Dataset Empty" << std::endl;
            return;
        }
        
        for(std::vector<Sortable *>::iterator i = s.begin(); i != s.end(); i++)
        {
            (*i)->print();
        }
        std::cout << std::endl;
    }

    void print()
    {
        print(data);
    }
 
    // use insertion sort
    void sort ()
    {
        std::vector<Sortable *> sorted;
        for(std::vector<Sortable *>::iterator i = data.begin(); i != data.end(); i++)
        {
            std::vector<Sortable *>::iterator l = sorted.begin();
            for(; l != sorted.end(); l++)
                if(! (*l)->compare(*i))
                    break;

            sorted.insert(l, *i);
        }

        std::swap(sorted, data);
    }
};

#endif

#ifndef __CIRCLE_H__
#define __CIRCLE_H__

class Circle : public Sortable
{
    float radius;

public:
    Circle() : radius(0)
    {
    }

    Circle(float r) : radius(r)
    {
    }
  
    void print()
    {
        std::cout << "Circle with radius: " << radius << std::endl;
    }

    bool compare(const Sortable *s)
    {
        const Circle *other = dynamic_cast<const Circle *>(s);

        return radius < other->radius;
    }
};

#endif

