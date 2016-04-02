#ifndef __PARTICIPANT_H__
#define __PARTICIPANT_H__

#include <iostream>
#include <iomanip>
#include "Sortable.h"

class Participant : public Sortable
{
public:
    
    std::string name;
    int         age;
    double      score;
    
    static bool (*comp_cb)(const Participant *, const Participant *);
    
    Participant(const std::string name, int age, double score)
        : name(name), age(age), score(score)
    {
    }
    
    void print()
    {
        std::cout << name << "\t" << age << "\t" << std::setprecision(2) << score << std::endl;
    }
        
    bool compare(const Sortable *s)
    {
        const Participant *other = dynamic_cast<const Participant *>(s);
        if(!other)
        {
            return false;
        }
        
        return comp_cb(other, this);
    }
};

bool (Participant::*comp_cb)(const Participant *, const Participant *);

#endif
