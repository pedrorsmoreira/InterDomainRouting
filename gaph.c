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
  
    // Initialize each adjacency list as empty by  
    // making head as NULL 
    int i; 
    for (i = 0; i < V; ++i) 
        graph->array[i].head = NULL;

    // Create arrays of size V that will be initialized with 0. 
    // The first will be the nodes that already have been visited, 
    // the second one te nodes that are not permited (this two 
    // arrays will be used in the DFS to check customer cyclyes)
    // and the last one to indicates the nodes that are Tier-1
    graph->visited = (int*) malloc(V * sizeof(int));
    memset(graph->visited, 0, V);
    graph->notPermited = (int*) malloc(V * sizeof(int));
    memset(graph->notPermited, 0, V);
    graph->tier1 = (int*) malloc(V * sizeof(int));
    memset(graph->tier1, 0, V);

    return graph; 
} 
  
// Adds an edge to an undirected graph 
void addEdge(struct Graph* graph, int src, int dest, int relation) 
{
    if (src >= MAXSIZE || dest >= MAXSIZE) {
        printf("Invalid values in the input file\n");
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
int printGraph(struct Graph* graph) 
{ 
    int v;
    int vertexes = 0;
    for (v = 0; v < graph->V; ++v) 
    { 
        struct AdjListNode* pCrawl = graph->array[v].head;

        printf("\n Adjacency list of vertex %d\n head", v); 
        
        if(pCrawl != NULL)
            ++vertexes;

        while (pCrawl) 
        { 
            printf(" -> %d|%d", pCrawl->dest, pCrawl->relation); 
            pCrawl = pCrawl->next; 
        }

        printf("\n"); 
    }
    return vertexes; 
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

/*
void DFS(struct Graph* graph, int vertex)
{
    struct AdjListNode* aux = graph->array[vertex].head;

    graph->visited[vertex] = 1;
    printf("Visited %d \n", vertex);

    while(aux != NULL) {
        
    }


}

bool checkCustomersCycles(struct Graph* graph)
{

}
*/

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









#define maxWT 4

int *wt; 
bool *st;

int LessNum(Item a, Item b)
{
    int aa, bb;

    aa = *((int *)a);
    bb = *((int *)b);

    return (wt[aa] > wt[bb]);
}

int GenDijkstra(struct Graph * graph, Heap *h_, int fakeSource)
{
    int explored_nodes = 0;
    Heap *h = NewHeap(MAXSIZE, LessNum);
    int *v, w, *vertecisPos = getHeapElementes_pos(h);
    struct AdjListNode* t;
    wt = (int *) malloc(MAXSIZE * sizeof(int));
    st = (bool *) malloc(MAXSIZE * sizeof(bool));

    for (int i = 0; i < graph->V; i++) {
        v = (int *) malloc(sizeof(int)); //fazer isto fora e depois mudar só e dar os "free(v)" só no fim
        if (v == NULL)      exit(0);
        *v = i;
        wt[i] = maxWT;
        st[i] = false;
        Direct_Insert(h, (Item) v);
        vertecisPos[i] = i;
    }

     wt[fakeSource] = 3;    //bc of lastcost condition
     FixUp(h, fakeSource);

    for(v = RemoveMax(h); wt[*v] != maxWT; v = RemoveMax(h)) {
        ++ explored_nodes;
        printf("DIJKSTRA %d\n", *v);
        
        st[*v] = true;
        unsigned int lastcost = wt[*v];
        if (lastcost == 2)
            --lastcost;
        for (t = graph->array[*v].head; t != NULL; t = t->next) { //printf("FFFOOOORRRR   %d\n", t->dest);
            if (!st[t->dest] && t->relation <= lastcost && wt[t->dest] > t->relation) { //printf("IIIIIIFFFFFF\n");
                wt[t->dest] = t->relation;
                FixUp(h, vertecisPos[t->dest]);
            }
        }

        free (v);
    }

    free(v);

    return explored_nodes;
}
