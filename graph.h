/* Prevent multiple inclusions                                      */
#ifndef GRAPH
#define GRAPH

#include <string.h>
#include <stdbool.h>

#include "heaps.h"

#define MAXSIZE 65536

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
    struct AdjListNode* head;  
}; 
  
// A structure to represent a graph. A graph 
// is an array of adjacency lists. 
// Size of array will be V (number of vertices  
// in graph) 
struct Graph 
{ 
    int V; 
    struct AdjList* array;
    int* tier1;
};

struct AdjListNode* newAdjListNode(int dest, int relation);

struct Graph* createGraph(int V);

void addEdge(struct Graph* graph, int src, int dest, int relation);

int printGraph(struct Graph* graph);


void freeAdjList(struct AdjListNode * list);

void freeGraph(struct Graph* graph);

bool checkCommercialConnectedness(struct Graph* graph);


int LessNum(Item a, Item b);

int GenDijkstra(struct Graph * graph, Heap *h, int fakeSource);

/* End if: Protect multiple inclusions                              */
#endif