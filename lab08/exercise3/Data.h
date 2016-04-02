#ifndef __DATA__
#define __DATA__

#include <iostream>
#include <vector>
#include <algorithm>
#include "Sortable.h"

class Data
{
    std::vector<Sortable *> data;

public:
    void add (Sortable *n)
    {
        data.push_back(n);
    }

    void print ()
    {
        for(std::vector<Sortable *>::iterator i = data.begin(); i != data.end(); i++)
        {
            (*i)->print();
            std::cout << " ";
        }
        std::cout << std::endl;
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

