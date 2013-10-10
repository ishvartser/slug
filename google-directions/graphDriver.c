//
//  graphDriver.c
//  Program 4: Google Directions
//  Professor D. Hembold
//  CMPS 101: Algorithms & Abstract Data Types
//  UC Santa Cruz  
//
//  Created by Jason Heron and Igor Shvartser on 5/25/13.
//

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "list.h"


int main()
{
    GraphRef G1 = newGraph(5);
    printf("Printing empty Graph of 10 verticies\n");
    printGraph(G1);
    
    addEdge(G1, 0, 1);
    addEdge(G1, 1, 2);
    addEdge(G1, 2, 3);
    addEdge(G1, 3, 4);
    addEdge(G1, 4, 0);
    
    printGraph(G1);
    doBFS(G1, 0);
    printGraph(G1);
    
    printf("\nDistance from 0 to 4 is: %d\n",getDistance(G1, 4));
    ListRef Path = getPathTo(G1, 4);
    printList(stdout, Path);
    
    freeList(&Path);
    freeGraph(G1);
    free(G1);
}