#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "api.h"

extern int providers;
extern int peers;
extern int customers;

extern int* wt;
extern bool* st;
extern int* lastcost;

extern int* prevHops;
extern int* totalHops;

extern bool* visited;
extern bool* notPermited;
extern int* tier1;

extern int ASesNumber;

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

        addEdge(graph, atoi(token), atoi(token2), MAXSIZE*atoi(token3)); 
    }

    fclose (fp);

    return graph;
}

void clearArrays()
{
    for (int i = 0; i < MAXSIZE; i++) {
        visited[i] = false;
        notPermited[i] = false;
    }
}

bool DFS(struct Graph* graph, int vertex)
{
    struct AdjListNode* aux = graph->array[vertex].customers;

    visited[vertex] = true;
    notPermited[vertex] = true;

    while(aux != NULL) {
        if (notPermited[aux->dest])
            return false;
        if (!visited[aux->dest]) 
            if (!DFS(graph, aux->dest))
                return false;
        aux = aux->next;
    }

    notPermited[vertex] = false;

    return true;
}

bool checkCustomersCycles(struct Graph* graph)
{
    for (int i = 0; i < MAXSIZE; i++){
        if (tier1[i] == 2) {
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
        if (tier1[i] == 2)
            for(int j = i + 1; j < MAXSIZE; j++)
                if (tier1[j] == 2) {
                    flag = 0;
                    aux = graph->array[i].peers;
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

Heap* setupDijkstra()
{
    for (int i = 0; i < MAXSIZE; ++i)
        if (tier1[i] > 0)
            ++ASesNumber;

    wt = (int *) malloc(MAXSIZE * sizeof(int));
    st = (bool *) malloc(MAXSIZE * sizeof(bool));
    lastcost = (int *) malloc(MAXSIZE * sizeof(int));

    prevHops = (int *) malloc(MAXSIZE * sizeof(int));
    totalHops = (int *) malloc(MAXSIZE * sizeof(int));
    
    // The 1 added to ASesNumber is for a bug correction 
    // in the dijkstra main cicle (this way the heap will 
    // always have a node with maxWT)
    Heap* h = NewHeap(ASesNumber + 1, MAXSIZE, LessNum);
    int* HeapPositions = getHeapElementes_pos(h);

    for (int i = 0, j = 0; i < MAXSIZE; ++i) {
        totalHops[i] = 0;

        if (tier1[i] == 0 && i > 0) {
            HeapPositions[i] = i;
            continue;
        }
        
        int* v = (int *) malloc(sizeof(int));
        if (v == NULL) exit(0);
        
        *v = i;
        Direct_Insert(h, (Item) v);
        HeapPositions[i] = j++;
    }

    return h;
}

int LessNum(Item a, Item b)
{
    int aa, bb;

    aa = *((int *)a);
    bb = *((int *)b);

    return ((wt[aa] - prevHops[aa]) < (wt[bb] - prevHops[bb]));
}

void scanList(Heap *h, int* HeapPositions, struct AdjListNode* aux, int source)
{
    while(aux) {
        if (!st[aux->dest] && aux->relation - prevHops[source] - 1 > wt[aux->dest] - prevHops[aux->dest]) {
            prevHops[aux->dest] = prevHops[source] + 1;
            //printf("source %d prevHops[source] %d aux->dest %d prevHops[aux->dest] %d\n", source, prevHops[source], aux->dest, prevHops[aux->dest]);
            wt[aux->dest] = aux->relation;
            lastcost[aux->dest] = aux->relation;
            FixUp(h, HeapPositions[aux->dest]);

            //printf("aux->dest é %d wt[aux->dest] é %d lastcost[aux->dest] %d\n", aux->dest, wt[aux->dest], lastcost[aux->dest]);
        }

        aux = aux->next;
    }
}

void GenDijkstra(struct Graph * graph, Heap *h, int fakeSource)
{
    resetHeapElementsNr(h);

    int* HeapPositions = getHeapElementes_pos(h);
    int* v = NULL; 

    --customers;

    for (int i = 0; i < graph->V; ++i) {
        if (tier1[i] == 0)
            continue;

        wt[i] = minWT*MAXSIZE;
        st[i] = false;
        prevHops[i] = 0;
        lastcost[i] = 3*MAXSIZE;
    }

    wt[fakeSource] = 3*MAXSIZE;
    FixUp(h, HeapPositions[fakeSource]);

    for(v = RemoveMax(h); wt[*v] != minWT; v = RemoveMax(h), st[*v] = true) {
        //printf("\nBeing Explored %d\n", *v);

        if (wt[*v] == 1*MAXSIZE)
        #ifdef COMMERCIAL
            break;
        #else
            ++providers;
        #endif

        else if (wt[*v] == 2*MAXSIZE)
            ++peers;
        else
            ++customers;

        ++totalHops[prevHops[*v]];

        if (lastcost[*v] == 3*MAXSIZE) {
            scanList(h, HeapPositions, graph->array[*v].providers, *v);
            scanList(h, HeapPositions, graph->array[*v].peers, *v);
        }
        scanList(h, HeapPositions, graph->array[*v].customers, *v);
    }
}

void scanListBFS(struct queue* q, struct AdjListNode* aux, bool condition, int currentVertex, int* totalHopsBFS)
{
    while(aux) {
        if(visited[aux->dest] == condition) {
            visited[aux->dest] = !condition;
            pushQueue(q, aux->dest);

            prevHops[aux->dest] = prevHops[currentVertex] + 1;
            ++totalHopsBFS[prevHops[aux->dest]];
        }
        aux = aux->next;
    }
}

bool BFS(struct Graph* graph, int startVertex, struct queue* q, bool condition, int* totalHopsBFS) 
{    
    visited[startVertex] = !condition;
    pushQueue(q, startVertex);

    prevHops[startVertex] = 0;
    
    while(!isEmpty(q)) {
        //printQueue(q);
        int currentVertex = popQueue(q);
        //printf("Visited %d\nprevHops[currentVertex] %d\n", currentVertex, prevHops[currentVertex]);

        scanListBFS(q, graph->array[currentVertex].providers,   condition, currentVertex, totalHopsBFS);
        scanListBFS(q, graph->array[currentVertex].peers,       condition, currentVertex, totalHopsBFS);
        scanListBFS(q, graph->array[currentVertex].customers,   condition, currentVertex, totalHopsBFS);
    }

    return !condition;
}

void lengthShortestPaths(struct Graph* graph)
{
    int i;
    bool condition = false;

    int* totalHopsBFS = (int *) malloc(MAXSIZE * sizeof(int));

    struct queue* q = createQueue();

    for (i = 0; i < graph->V; ++i) {
        visited[i] = condition;
        totalHopsBFS[i] = 0;
    }
    
    for (i = 0; i <= ITERATIONS; ++i)
        if (tier1[i] > 0)
            condition = BFS(graph, i, q, condition, totalHopsBFS);
        

    printf("\n---------- SHORTEST PATHS ----------\n");

    int counter = 0;
    for (int i = 0; i < 12; ++i)
        counter += totalHopsBFS[i];

    printf("counter %d\n", counter);

    int sum = 0;
    for (int i = 1; i < 12; ++i) 
        if (totalHopsBFS[i]) {
            sum += totalHopsBFS[i-1];
            printf("P(X>=%d) = %f\n", i, ((double)(counter - sum)/counter)*100);
        }

    free(q);  
}