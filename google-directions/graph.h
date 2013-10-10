//
//  graph.h
//  Program 4: Google Directions
//  Professor D. Hembold
//  CMPS 101: Algorithms & Abstract Data Types
//  UC Santa Cruz  
//
//  Created by Jason Heron and Igor Shvartser on 5/25/13.
//

#include "list.h"
#ifndef Program_4_graph_h
#define Program_4_graph_h

/**********Structures**********/

typedef struct Graph* GraphRef;

/**********Constructor/Destructor**********/

//constructs a new graph with n verticies
GraphRef newGraph(int n);

//returns resources used by graph to heap
void freeGraph(GraphRef G);

/**********Manipulators**********/

//adds an edge to the graph
void addEdge(GraphRef G, int from, int to);

/**********Accessors**********/

//does a BFS from the source
void doBFS(GraphRef G, int source);

//using previous BFS data, gets the distance to "destination"
//returns -1 if there is no path
int getDistance(GraphRef G, int destination);

//returns a list containing the path from the last doBFS source to "destination"
//returns NULL if no path exists
ListRef getPathTo(GraphRef G, int destination);

//prints the state of the graph
void printGraph(GraphRef G);




#endif
