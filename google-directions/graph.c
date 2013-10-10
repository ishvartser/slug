//
//  graph.c
//  Program 4: Google Directions
//  Professor D. Hembold
//  CMPS 101: Algorithms & Abstract Data Types
//  UC Santa Cruz  
//
//  Created by Jason Heron and Igor Shvartser on 5/25/13.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"
#include "list.h"

/**********Prototypes**********/
void cleanGraph(GraphRef G);


/**********Structures**********/
typedef struct Graph
{
    int totalVert;
    ListRef *adjacency;
    int *distance;
    int *parent;
    int *color;
}Graph;


/**********Helper Functions**********/
//resets a graph back to init state
void cleanGraph(GraphRef G)
{
    int i;
    for(i=0;i<G->totalVert;i++)
    {
        G->color[i] = 0;
        G->distance[i] = 0;
        G->parent[i] = -1;
    }
}


/**********Constructor/Destructor**********/
//creates a new graph with n verticies
GraphRef newGraph(int n)
{
    int i;
    GraphRef G = malloc(sizeof(Graph));
    G->adjacency = malloc(sizeof(ListRef)*n);
    G->color = calloc(sizeof(int),n);
    G->distance = calloc(sizeof(int),n);
    G->parent = malloc(sizeof(int)*n);
    G->totalVert = n;
    for(i=0; i<n; i++)
    {
        G->adjacency[i] = newList();
        G->parent[i] = -1;
    }
    return G;
}

//frees up resources dedicated to the graph
void freeGraph(GraphRef G)
{
    int i;
    free(G->parent);
    free(G->distance);
    free(G->color);
    for(i=0; i<G->totalVert; i++)
    {
        freeList(&G->adjacency[i]);
    }
    free(G->adjacency);
}


/**********Manipulators**********/
//adds an edge to the graph from "from" to "to"
void addEdge(GraphRef G, int from, int to)
{
    insertAfterLast(G->adjacency[from], (long)to);
}


/**********Accessors**********/
//does a BFS from the source
void doBFS(GraphRef G, int source)
{
    //setup
    int u,v;
    ListRef queue = newList();
    cleanGraph(G);
    
    //color source white
    G->color[source] = 1; // THIS SHOULD PROBABLY = 0
    
    //put source in the queue
    insertAfterLast(queue, source);
    while(!isEmpty(queue))
    {
        //dequeue first element
        u = (int)getFirst(queue);
        deleteFirst(queue);
        
        //walk through adj-list
        moveFirst(G->adjacency[u]);
        while(!offEnd(G->adjacency[u]))
        {
            //get current vertex to examine
            v = (int)getCurrent(G->adjacency[u]);
            
            //if vertex is white
            if(G->color[v] == 0)
            {
                //turn vertex grey and update graph and queue
                G->color[v] = 1;
                G->distance[v] = (G->distance[u]+1);
                G->parent[v] = u;
                insertAfterLast(queue, v);
            }
            moveNext(G->adjacency[u]);
        }
    }
    freeList(&queue);
}

//using previous BFS data, gets the distance to "destination"
//returns -1 if there is no path
int getDistance(GraphRef G, int destination)
{
    if(G->distance[destination] == 0)
        return -1;
    return G->distance[destination];
}

//using previous BFS data, returns a list containing the path from the "source"
//to the "destination", returns NULL if no path exists
ListRef getPathTo(GraphRef G, int destination)
{
    int i;
    int dist = getDistance(G, destination);
    ListRef stack = newList();
    insertAfterLast(stack, destination);
    for(i=0; i<dist; i++)
    {
        insertBeforeFirst(stack, G->parent[destination]);
        destination = G->parent[destination];
    }
    return stack;
}

//prints the state of the graph
void printGraph(GraphRef G)
{
    int i;
    printf("\nState of graph! Total Verticies:%d\n", G->totalVert);
    for(i=0; i<G->totalVert; i++)
    {
        printf("Vertex:%d, Color:%d, parent:%d, distance:%d\n", i, G->color[i]
               , G->parent[i], G->distance[i]);
        printf("Adjacency: ");
        printList(stdout, G->adjacency[i]);
    }
}
