#include <iostream>
#include <vector>
#include "Graph.h"

using namespace std;


int main(int argc, const char * argv[])
{
    /*! Creating some vertices */
	Vertex * v1 = new Vertex("v1");
	Vertex * v2 = new Vertex("v2");
	Vertex * v3 = new Vertex("v3");
    Vertex * v4 = new Vertex("v4");
    Vertex * v5 = new Vertex("v5");
	
    /*! Storing the edges as pairs of vertices*/
    Graph edges;
    edges.add(v1);  //v1 is connected to v1
	edges.add(v1);
	
    edges.add(v1);  //v1 is connected to v3
	edges.add(v3);
    
    edges.add(v1);  // etc...
    edges.add(v4);
    
    edges.add(v1);
	edges.add(v2);
	
    edges.add(v1);
	edges.add(v3);
    
    edges.add(v1);
    edges.add(v4);
    
    edges.add(v2);
	edges.add(v3);
	
    edges.add(v2);
	edges.add(v4);
    
    edges.add(v3);
    edges.add(v4);
    
    edges.add(v5);
    edges.add(v1);
    
    edges.add(v5);
    edges.add(v3);
    
    /*! Display all the edges of the graph */
	edges.print();
    
    /*! Delete the whole graph by freeing memory taken up by vertices */
    edges.deleteAll();

    return 0;
}

