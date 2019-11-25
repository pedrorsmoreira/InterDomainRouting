/* Prevent multiple inclusions                                      */
#ifndef GRAPH
#define GRAPH

#include <string.h>
#include <stdbool.h>

#define MAXSIZE 12//65536

// A structure to represent an adjacency list node 
struct AdjListNode 
{ 
    int dest;
    int commercialRelationship;
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

struct AdjListNode* newAdjListNode(int dest, int commercialRelationship);

struct Graph* createGraph(int V);

void addEdge(struct Graph* graph, int src, int dest, int commercialRelationship);

void printGraph(struct Graph* graph);

<<<<<<< HEAD
void freeAdjList(struct AdjListNode * list);

void freeGraph(struct Graph* graph);
=======
bool checkCommercialConnectedness(struct Graph* graph);
>>>>>>> 493315e4ed06721353837d663907f0fcc98ced38

/* End if: Protect multiple inclusions                              */
#endif