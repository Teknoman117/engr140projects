#ifndef __PARTICIPANT_H__
#define __PARTICIPANT_H__

#include <iostream>
#include <iomanip>
#include "Sortable.h"

class Participant : public Sortable
{
    std::string name;
    int         age;
    double      score;
    
public:
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
        if(other)
        {
            if(score == other->score)
            {
                if(age == other->age)
                {
                    return name.compare(other->name) < 0;
                }
                else return age < other->age;
            }
            else return score > other->score;
        }
        
        return false;
    }
};

#endif
