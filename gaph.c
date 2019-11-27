#include <stdio.h>
#include <stdlib.h>


#include "graph.h"

extern int providers;
extern int peers;
extern int customers;

int* wt = NULL;
bool* st = NULL;
unsigned int* lastcost = NULL;
  
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

    // Initialize each adjacency list as empty by  
    // making head as NULL and initialize the rest
    // of the arrays
    int i; 
    for (i = 0; i < V; ++i) {
        graph->array[i].head = NULL;
        graph->visited[i] = false;
        graph->notPermited[i] = false;
        graph->tier1[i] = 0;
    }

    return graph; 
} 
  
// Adds an edge to an undirected graph 
void addEdge(struct Graph* graph, int src, int dest, int relation) 
{
    if (src >= MAXSIZE || dest >= MAXSIZE) {
        //printf("Invalid values in the input file\n");
        exit(1);
    }

    // If this happen is not an AS of Tier-1
    if (relation == 3)
        graph->tier1[src] = 1;
    else
        if (graph->tier1[src] == 0)
            graph->tier1[src] = 2;

    // Add an edge from src to dest.  A new node is  
    // added to the adjacency list of src.  The node 
    // is added at the begining 
    struct AdjListNode* newNode = newAdjListNode(dest, relation); 
    newNode->next = graph->array[src].head; 
    graph->array[src].head = newNode;
} 

// A utility function to print the adjacency list  
// representation of graph 
void printGraph(struct Graph* graph) 
{ 
    int v;
    for (v = 0; v < graph->V; ++v) { 
        struct AdjListNode* pCrawl = graph->array[v].head;
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
    for (int i = 0;  i < graph->V; ++i)
        freeAdjList(graph->array[i].head);
}

struct Graph* readFile(char* filename)
{
    FILE *fp = fopen (filename, "r");
    if (fp == NULL) {
        perror (filename);
        exit(1);
    }

    // APAGUEI O +1 - CONFIRMAR QUE NAO HA STRESS
    //the 1 added to MAXSIZE is for a bug correction in the dijkstra main cicle (this way the heap will always have a node with maxWT)
    struct Graph* graph = createGraph(MAXSIZE);

    char line[128]; /* maximum line size */

    // read a line
    while (fgets (line, sizeof line, fp) != NULL) { 
        char *token  = strtok (line, " "); // Returns first token
        char *token2 = strtok (NULL, " "); // Returns second token
        char *token3 = strtok (NULL, " "); // Returns third token

        //printf("%d %d %d\n", atoi(token), atoi(token2), atoi(token3));

        if ((token == NULL) && (token2 == NULL) && (token3 == NULL)) {
            printf("Input file in the wrong structure\n");
            exit(1);
        }

        addEdge(graph, atoi(token), atoi(token2), atoi(token3)); 
    }

    fclose (fp);

    return graph;
}


void clearArrays(struct Graph* graph)
{
    for (int i = 0; i < graph->V; i++) {
        graph->visited[i] = false;
        graph->notPermited[i] = false;
    }
}

bool DFS(struct Graph* graph, int vertex)
{
    struct AdjListNode* aux = graph->array[vertex].head;

    graph->visited[vertex] = true;
    graph->notPermited[vertex] = true;

    while(aux != NULL) {
        if (aux->relation == 1) {
            if (graph->notPermited[aux->dest] == true)
                return false;
            if (graph->visited[aux->dest] == false) {
                if (!DFS(graph, aux->dest))
                    return false;
            }

        }
        aux = aux->next;
    }

    graph->notPermited[vertex] = false;

    return true;
}

bool checkCustomersCycles(struct Graph* graph)
{
    for (int i = 0; i < MAXSIZE; i++){
        if (graph->tier1[i] == 2) {
            if (!DFS(graph, i))
                return false;
            clearArrays(graph);
        }
    }

    return true;
}

bool checkCommercialConnectedness(struct Graph* graph)
{
    int flag = 0;
    struct AdjListNode* aux = NULL;

    for (int i = 0; i < MAXSIZE; i++)
        if (graph->tier1[i] == 2)
            for(int j = i + 1; j < MAXSIZE; j++)
                if (graph->tier1[j] == 2) {
                    flag = 0;
                    aux = graph->array[i].head;
                    while(aux != NULL) {
                        if (aux->dest == j) {
                            flag = 1;
                            break;
                        }
                        aux = aux->next;
                    }

                    if (flag == 0)
                        return false;
                }

    return true;
}

int LessNum(Item a, Item b)
{
    int aa, bb;

    aa = *((int *)a);
    bb = *((int *)b);

    return (wt[aa] < wt[bb]);
}

int GenDijkstra(struct Graph * graph, Heap *h, int fakeSource)
{
    resetHeapElementsNr(h);
    int explored_nodes = 0;

    int* HeapPositions = getHeapElementes_pos(h);
    int* v = NULL; 
    struct AdjListNode* t = NULL;

    for (int i = 0; i < graph->V; ++i) {
        if (graph->tier1[i] == 0)
            continue;

        wt[i] = minWT;
        st[i] = false;
        lastcost[i] = 3;
    }

    wt[fakeSource] = 3;
    FixUp(h, HeapPositions[fakeSource]);

    for(v = RemoveMax(h); wt[*v] != minWT; v = RemoveMax(h), st[*v] = true) {
        if (*v != fakeSource) {
            if (wt[*v] == 1)
                break;
            else if (wt[*v] == 2)
                ++peers;
            else
                ++customers;

            ++explored_nodes;
        }

        for (t = graph->array[*v].head; t != NULL; t = t->next) {
            if (!st[t->dest] && t->relation <= lastcost[*v] && t->relation > wt[t->dest]){
                wt[t->dest] = t->relation;
                lastcost[t->dest] = (t->relation == 2) ? t->relation - 1 : t->relation;
                FixUp(h, HeapPositions[t->dest]);

                //printf("t->dest é %d wt[t->dest] é %d lastcost[t->dest] %d\n", t->dest, wt[t->dest], lastcost[t->dest]);
            }
        }
    }

    return explored_nodes;
}