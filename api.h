/* Prevent multiple inclusions                                      */
#ifndef API
#define API
 
#include "graph.h"
#include "heaps.h"
#include "queue.h"

#define minWT 0

struct Graph* readFile(char* filename);

void clearArrays(struct Graph* graph);

bool DFS(struct Graph* graph, int vertex);

bool checkCustomersCycles(struct Graph* graph);

bool checkCommercialConnectedness(struct Graph* graph);


int LessNum(Item a, Item b);

int GenDijkstra(struct Graph * graph, Heap *h, int fakeSource);


void lengthShortestPaths(struct Graph* graph);

bool BFS(struct Graph* graph, int startVertex, struct queue* q, bool condition);

/* End if: Protect multiple inclusions                              */
#endif