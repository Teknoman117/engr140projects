#ifndef LA9_Graph_h
#define LA9_Graph_h

#include "Vertex.h"
#include <iostream>

using namespace std;

class Graph {
public:
    
    /*! A collection of vertices */
    vector<Vertex*> items;
    
    /*! A method to add a vertex to the items vector */
    void add(Vertex* d)
    {
        //insert your code here
        d->ref();
        items.push_back(d);
    }
    
    /*! A method to display the graph as pairs of vertices */
    void print ()
    {
        for(vector<Vertex*>::iterator it = items.begin(); it != items.end(); it+=2)
        {
            cout << (*it)->getLabel() << " - " << (*(it+1))->getLabel() << endl;
        }
    }
    
    /*! A method to free up all the space taken up by the graph */
    void deleteAll(){
        //insert your code here
        for(vector<Vertex*>::iterator it = items.begin(); it != items.end(); it++)
            (*it)->unref();
    }
};

#endif
