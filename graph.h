/* Prevent multiple inclusions                                      */
#ifndef GRAPH
#define GRAPH

#define MAXSIZE 12//65536

// A structure to represent an adjacency list node 
struct AdjListNode 
{ 
    int dest;
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
};

struct AdjListNode* newAdjListNode(int dest);

struct Graph* createGraph(int V);

void addEdge(struct Graph* graph, int src, int dest);

void printGraph(struct Graph* graph);

/* End if: Protect multiple inclusions                              */
#endif