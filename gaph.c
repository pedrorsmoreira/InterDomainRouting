#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
  
// A utility function to create a new adjacency list node 
struct AdjListNode* newAdjListNode(int dest, int commercialRelationship) 
{ 
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode)); 
    newNode->dest = dest;
    newNode->commercialRelationship = commercialRelationship; 
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
  
    // Initialize each adjacency list as empty by  
    // making head as NULL 
    int i; 
    for (i = 0; i < V; ++i) 
        graph->array[i].head = NULL; 
  
    return graph; 
} 
  
// Adds an edge to an undirected graph 
void addEdge(struct Graph* graph, int src, int dest, int commercialRelationship) 
{
    if (src >= MAXSIZE || dest >= MAXSIZE) {
        printf("Invalid values in the input file\n");
        exit(1);
    }

    // Add an edge from src to dest.  A new node is  
    // added to the adjacency list of src.  The node 
    // is added at the begining 
    struct AdjListNode* newNode = newAdjListNode(dest, commercialRelationship); 
    newNode->next = graph->array[src].head; 
    graph->array[src].head = newNode;
} 

// A utility function to print the adjacency list  
// representation of graph 
void printGraph(struct Graph* graph) 
{ 
    int v; 
    for (v = 0; v < graph->V; ++v) 
    { 
        struct AdjListNode* pCrawl = graph->array[v].head; 
        printf("\n Adjacency list of vertex %d\n head", v); 
        while (pCrawl) 
        { 
            printf(" -> %d|%d", pCrawl->dest, pCrawl->commercialRelationship); 
            pCrawl = pCrawl->next; 
        } 
        printf("\n"); 
    } 
}

void freeAdjList(struct AdjListNode * list){
    while (list != NULL){
        struct AdjListNode *aux = list->next;
        free(list);
        list = aux;
    }
}

void freeGraph(struct Graph* graph){
    for (int i = 0;  i < graph->V; ++i)
        freeAdjList(graph->array[i].head);
}

#define maxWT 10000
#define P (wt[*v] + ((node*)(t->this))->cost)

int st[], int wt[];

void GenDijkstra(struct Graph * graph, Heap *h, int source)
{
    int *v, w, *vertecisPos = getHeapElementes_pos(h);
    LinkedList *t;

    for (int i = 0; i < graph->V; i++) {
        v = (int *)malloc(sizeof(int));
        if (v == NULL)      exit(0);
        *v = i;
        st[i] = -1;
        wt[i] = maxWT;
        Direct_Insert(h, (Item) v);
        vertecisPos[i] = i;
    }

     wt[source] = 0;
     FixUp(h, source);

    for(v = RemoveMax(h); *v != destination && wt[*v] != maxWT; v = RemoveMax(h)){
        for (t = Graph[*v]; t != NULL; t = t->next)
            if (((node*)(t->this))->cost <= maxCost && wt[w = ((node*)(t->this))->vertex] > P){
            wt[w] = P;
            FixUp(h, vertecisPos[w]);
            st[w] = *v;
            }
     free (v);
    }

    free(v);
}