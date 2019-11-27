#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "api.h"

extern int providers;
extern int peers;
extern int customers;

int* wt = NULL;
bool* st = NULL;
unsigned int* lastcost = NULL;

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


bool BFS(struct Graph* graph, int startVertex, struct queue* q, bool condition) 
{    
    graph->visited[startVertex] = !condition;
    pushQueue(q, startVertex);

    graph->counterHops[startVertex] = 0;
    
    while(!isEmpty(q)) {
        //printQueue(q);
        int currentVertex = popQueue(q);
        //printf("Visited %d\ngraph->counterHops[currentVertex] %d\n", currentVertex, graph->counterHops[currentVertex]);

        struct AdjListNode* aux = graph->array[currentVertex].head;
        while(aux) {
            if(graph->visited[aux->dest] == condition) {
                graph->visited[aux->dest] = !condition;
                pushQueue(q, aux->dest);

                graph->counterHops[aux->dest] = graph->counterHops[currentVertex] + 1;
                ++graph->totalHops[graph->counterHops[aux->dest]];
            }
            aux = aux->next;
       }
    }

    return !condition;
}

void lengthShortestPaths(struct Graph* graph)
{
    struct queue* q = createQueue();

    bool condition = false;
    int i;
    for (i = 0; i < graph->V; ++i)
        graph->visited[i] = condition;
    
    for (i = 0; i <= ITERATIONS; ++i) {
    //for (int i =1; i <= MAXSIZE; ++i) {
        if (graph->tier1[i] > 0) {
            condition = BFS(graph, i, q, condition);
        }
    }

    int counter = 0;
    printf("\n");
    for (i = 0; i < 12; ++i)
    {
        printf("i %d graph->totalHops[i] %d\n", i, graph->totalHops[i]);
        counter += graph->totalHops[i];
    }

    printf("counter %d\n", counter);

    free(q);  
}