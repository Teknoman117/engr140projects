#ifndef __DATA__
#define __DATA__

#include <iostream>
#include <vector>
#include <algorithm>

class Data
{
    std::vector<int> data;

public:
    void add (int number)
    {
        data.push_back(number);
    }

    void print () const
    {
        for(std::vector<int>::const_iterator i = data.begin(); i != data.end(); i++)
        {
            std::cout << *i << " ";
        }
        std::cout << std::endl;
    }
 
    // use insertion sort
    void sort ()
    {
        std::vector<int> sorted;
        for(std::vector<int>::iterator i = data.begin(); i != data.end(); i++)
        {
            std::vector<int>::iterator l = std::upper_bound(sorted.begin(), sorted.end(), *i);
            sorted.insert(l, *i);    
        }

        std::swap(sorted, data);
    }
};

#endif

