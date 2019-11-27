#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <float.h>

#include <time.h>

#include "graph.h"
#include "api.h"

void print_usage(char*);

int providers = 0;
int peers = 0;
int customers = 0;

const double lowest_double = DBL_MIN;

extern int *wt;
extern bool *st;
extern unsigned int *lastcost;

int main(int argc, char *argv[]) 
{ 
	printf("lowest_double %lf\n", lowest_double);

	int opt;
	char* filename = NULL;

	if (argc < 2) {
		print_usage(argv[0]);
		exit(1);
	}

	while ((opt = getopt(argc, argv, "f:")) != -1) {
		switch (opt) {
			case 'f':
				filename = optarg;
				break;

			default:
				print_usage(argv[0]);
		}
	}

	clock_t begin = clock();

	/********** CREATE GRAPH AND CHECKING OF PROPERTIES **********/

	struct Graph* graph = readFile(filename);

	if (!checkCustomersCycles(graph))
		printf("HAVE CUSTOMER CYCLES\n");

	if (!checkCommercialConnectedness(graph))
		printf("NOT COMMERCIAL CONNECTED\n");

	//printGraph(graph);
	//printf("\n");

	/********** DIJKSTRA **********/

	/*

	int ASesNumber = 0;
	for (int i = 0; i < graph->V; ++i)
		if (graph->tier1[i] > 0)
            ++ASesNumber;

	wt = (int *) malloc(MAXSIZE * sizeof(int));
    st = (bool *) malloc(MAXSIZE * sizeof(bool));
    lastcost = (unsigned int *) malloc(MAXSIZE * sizeof(unsigned int));
	
	// The 1 added to ASesNumber is for a bug correction 
	// in the dijkstra main cicle (this way the heap will 
	// always have a node with maxWT)
	Heap* h = NewHeap(ASesNumber + 1, MAXSIZE, LessNum);
	int* HeapPositions = getHeapElementes_pos(h);

	for (int i = 0, j = 0; i < graph->V; ++i) {
		if (graph->tier1[i] == 0 && i > 0){
			HeapPositions[i] = i;
            continue;
        }
        int* v = (int *) malloc(sizeof(int));
        if (v == NULL) exit(0);
        
        *v = i;
        Direct_Insert(h, (Item) v);
        HeapPositions[i] = j++;
    }

    printf("SETUP DONE\n");

	for (int i = 0; i <= 100; ++i) {
	//for (int i =1; i <= ITERATIONS; ++i) {
		if (graph->tier1[i] > 0)
			// Using the -1 because we should not count with the node
			// from where the Dijkstra is launched
			providers += ASesNumber - 1 - GenDijkstra(graph, h, i);
	}

	*/

	lengthShortestPaths(graph);

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	/********** FREE MEMORY **********/

	/*
	free(wt); free(st);
	free(lastcost);
	FreeHeap(h);
	freeGraph(graph);
	*/

	/********** PRINTS **********/

	printf("\n");
	
	//printf("total ASes: %d\n", ASesNumber);
	printf("providers: %d\n", providers);
	printf("peers: %d\n", peers);
	printf("customers: %d\n", customers);
	int totalRoutes = providers + peers + customers;
	printf("total routes: %d\n", totalRoutes);

	printf("\n");

	printf("providers route: %f\n", ((double)providers/totalRoutes)*100);
	printf("peers route: %f\n", ((double)peers/totalRoutes)*100);
	printf("customers route: %f\n", ((double)customers/totalRoutes)*100);

	printf("\n");

	printf("Elapsed time: %f seconds\n\n", time_spent);

	printf("Elapsed time in minutes would be in total (est) = %d\n", ((MAXSIZE*time_spent)/INTERATIONS)/60);

	return 0;  
}

void print_usage(char *program)
{
	fprintf(stderr, "Usage: %s -f <input-file>\n", program);
	exit(1);
}