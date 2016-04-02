#include <iostream>
#include <string>
#include "Data.h"

class stringy : public Sortable
{
public:
    std::string content;

    stringy(const std::string& c) : content(c) {}

    bool compare(const Sortable* s)
    {
        const stringy *derp = dynamic_cast<const stringy*>(s);
        return content.compare(derp->content) < 0;
    }

    void print()
    {
        std::cout << content;
    }
};

int main ()
{
    Data sorter;
    sorter.add(new stringy("c"));
    sorter.add(new stringy("b"));
    sorter.add(new stringy("herpderp"));
    sorter.add(new stringy("a"));


    sorter.print();
    sorter.sort();
    sorter.print();

    return 0;
}
