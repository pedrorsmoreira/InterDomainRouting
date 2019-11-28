/* Prevent multiple inclusions                                      */
#ifndef GRAPH
#define GRAPH

#include <stdbool.h>

#include "defs.h"

// A structure to represent an adjacency list node 
struct AdjListNode 
{ 
    int dest;
    int relation;
    struct AdjListNode* next; 
};
  
// A structure to represent an adjacency list 
struct AdjList 
{
    struct AdjListNode* providers;
    struct AdjListNode* peers;
    struct AdjListNode* customers;
}; 
  
// A structure to represent a graph. A graph 
// is an array of adjacency lists. 
// Size of array will be V (number of vertices  
// in graph) 
struct Graph 
{ 
    int V; 
    struct AdjList* array;
};

struct AdjListNode* newAdjListNode(int dest, int relation);

struct Graph* createGraph(int V);

void addEdge(struct Graph* graph, int src, int dest, int relation);

void printGraph(struct Graph* graph);

void freeAdjList(struct AdjListNode * list);

void freeGraph(struct Graph* graph);

/* End if: Protect multiple inclusions                              */
#endif