/* Prevent multiple inclusions                                      */
#ifndef API
#define API
 
#include "graph.h"
#include "heaps.h"
#include "queue.h"

#define minWT 0


//uncomment to only get statistics from the commercial optimal routes
//#define COMMERCIAL

struct Graph* readFile(char* filename);

void clearArrays();

bool DFS(struct Graph* graph, int vertex);

bool checkCustomersCycles(struct Graph* graph);

bool checkCommercialConnectedness(struct Graph* graph);


Heap* setupDijkstra();

int LessNum(Item a, Item b);

void scanList(Heap *h, int* HeapPositions, struct AdjListNode* aux, int source);

void GenDijkstra(struct Graph * graph, Heap *h, int fakeSource);


void scanListBFS(struct queue* q, struct AdjListNode* aux, bool condition, int currentVertex, int* totalHopsBFS);

bool BFS(struct Graph* graph, int startVertex, struct queue* q, bool condition, int* totalHopsBFS);

void lengthShortestPaths(struct Graph* graph);

/* End if: Protect multiple inclusions                              */
#endif