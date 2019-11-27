#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
  
// A utility function to create a new adjacency list node 
struct AdjListNode* newAdjListNode(int dest, int relation) 
{ 
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode)); 
    newNode->dest = dest;
    newNode->relation = relation; 
    newNode->next = NULL; 
    return newNode; 
} 
  
// A utility function that creates a graph of V vertices 
struct Graph* createGraph(int V) 
{ 
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph)); 
    graph->V = V;
  
    // Create an array of adjacency lists.  Size of  
    // array will be V 
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList)); 
  
    // Create arrays of size V. The first will be the nodes 
    // that already have been visited, the second one te 
    // nodes that are not permited (this two arrays will be 
    // used in the DFS to check customer cyclyes) and the 
    // last one to indicates the nodes that are Tier-1
    graph->visited = (bool*) malloc(V * sizeof(bool));
    graph->notPermited = (bool*) malloc(V * sizeof(bool));
    graph->tier1 = (int*) malloc(V * sizeof(int));

    graph->counterHops = (int*) malloc(V * sizeof(int));
    graph->totalHops = (int*) malloc(V * sizeof(int));


    // Initialize each adjacency list as empty by  
    // making head as NULL and initialize the rest
    // of the arrays
    int i; 
    for (i = 0; i < V; ++i) {
        graph->array[i].providers = NULL;
        graph->array[i].peers = NULL;
        graph->array[i].customers = NULL;

        graph->visited[i] = false;
        graph->notPermited[i] = false;
        graph->tier1[i] = 0;

        graph->counterHops[i] = 0;
        graph->totalHops[i] = 0;
    }

    return graph; 
} 
  
// Adds an edge to an undirected graph 
void addEdge(struct Graph* graph, int src, int dest, int relation) 
{
    if (src >= MAXSIZE || dest >= MAXSIZE) {
        printf("Invalid values in the input file\n");
        exit(1);
    }

    // Add an edge from src to dest. A new node is added 
    // to the corresponding adjacency list of src according
    // to the relation. The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest, relation); 

    // If this happen is not an AS of Tier-1
    if (relation == 3*MAXSIZE) {
        newNode->next = graph->array[src].providers; 
        graph->array[src].providers = newNode;

        graph->tier1[src] = 1;    
    } else {
        if (relation == 2*MAXSIZE) {
            newNode->next = graph->array[src].peers;
            graph->array[src].peers = newNode;
        } else {
            newNode->next = graph->array[src].customers;
            graph->array[src].customers = newNode;
        }
        if (graph->tier1[src] == 0)
            graph->tier1[src] = 2;
    }
} 

// A utility function to print the adjacency list  
// representation of graph 
void printGraph(struct Graph* graph) 
{ 
    struct AdjListNode* pCrawl = NULL;

    int v;
    for (v = 0; v < ITERATIONS; ++v) {
        printf("\n\n--------------------\nVERTEX: %d\n\n", v);

        printf("providers\n");
        pCrawl = graph->array[v].providers;
        while (pCrawl) { 
            printf(" -> %d|%d", pCrawl->dest, pCrawl->relation); 
            pCrawl = pCrawl->next; 
        }

        printf("\npeers\n");
        pCrawl = graph->array[v].peers;
        while (pCrawl) { 
            printf(" -> %d|%d", pCrawl->dest, pCrawl->relation); 
            pCrawl = pCrawl->next; 
        }

        printf("\ncustomers\n");
        pCrawl = graph->array[v].customers;
        while (pCrawl) { 
            printf(" -> %d|%d", pCrawl->dest, pCrawl->relation); 
            pCrawl = pCrawl->next; 
        }
    }
}

void freeAdjList(struct AdjListNode * list)
{
    while (list != NULL){
        struct AdjListNode *aux = list->next;
        free(list);
        list = aux;
    }
}

void freeGraph(struct Graph* graph)
{
    for (int i = 0;  i < graph->V; ++i) {
        freeAdjList(graph->array[i].providers);
        freeAdjList(graph->array[i].peers);
        freeAdjList(graph->array[i].customers);
    }
}