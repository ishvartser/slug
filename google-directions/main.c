//
//  main.c
//  Program 4: Google Directions
//  Professor D. Hembold
//  CMPS 101: Algorithms & Abstract Data Types
//  UC Santa Cruz  
//
//  Created by Jason Heron and Igor Shvartser on 5/25/13.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "list.h" 

/*********globals*********/
static int numVerts,numQueries,from,to,base=10;
static char delimiters[] = ". ;,\n";

/*********debugging*********/
static int debug_input=0;
static int debug_querrie=0;
static int debug_graph=0;

/*********prototypes*********/
GraphRef createGraph(FILE *f);
int performSearch(GraphRef G,int from, int to);

/***************************/
int main(int argc, const char * argv[])
{
    int i;
    //open file
    //FILE *f=fopen(argv[1], "r");
    FILE *f = fopen("/Users/jason/Downloads/prog4Data/test3", "r");
    
    //scan in calibration data
    fscanf(f,"%d",&numVerts);
    fscanf(f,"%d",&numQueries);
    if(debug_input) printf("\nnumVerts:%d, numQueries:%d\n",numVerts,numQueries);
    
    //create populated graph structure
    GraphRef G = createGraph(f);
    if(debug_graph) printGraph(G);
    
    //handle querries
    for(i=0;i<numQueries;i++)
    {
        fscanf(f,"%d",&from);
        fscanf(f,"%d",&to);
        if(debug_querrie) printf("\n\nQuerrie:%d, from:%d, to:%d\n",i,from,to);
        if(!performSearch(G,from,to))
            printf("No valid path from %d to %d\n",from, to);
    }
    
    //clean up heap
    freeGraph(G);
    free(G);
    fclose(f);
}

//creates a graph based on input file
GraphRef createGraph(FILE *f)
{
    GraphRef G = newGraph(numVerts);
    int currentVert, subVert, i;
    char * token, * end, currentLine[numVerts];
    fgets(currentLine, numVerts , f); //consumes extra newline character
    
    //loops over each vert and creates adj-lists
    for(i=0; i<numVerts; i++)
    {
        fgets(currentLine, (numVerts*99) , f);
        token = strtok(currentLine, delimiters);
        currentVert = (int)strtol(token, &end, base);
        if(debug_input) printf("\nVert:%d Connected to:", currentVert);
        
        //loops over everything connected to currentVert and makes adj-list
        while(1)
        {
            //read in list elements
            token = strtok(NULL, delimiters);
            if (token == NULL) break;
            subVert = (int)strtol(token, &end, base);
            
            //add to adj-list
            addEdge(G, currentVert, subVert);
            if(debug_input)printf(" %d", subVert);
        }
    }
    return G;
}

//performs a BFS from "from" to "to"
int performSearch(GraphRef G, int from, int to)
{
    doBFS(G, from);
    
    //handle infinite distance case
    if(getDistance(G, to) == -1)
    return 0;
    
    //handle expected case
    ListRef path = getPathTo(G, to);
    printf("To get from %d to %d requires %d edges, follow the path: "
           ,from, to, getDistance(G, to));
    printList(stdout, path);
    
    //clean up
    freeList(&path);
    return 1;
}