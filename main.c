#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <time.h>

#include "graph.h"
#include "api.h"

void print_usage(char*);

int providers = 0;
int peers = 0;
int customers = 0;

int* wt = NULL;
bool* st = NULL;
int* lastcost = NULL;

int* prevHops = NULL;
int* totalHops = NULL;

bool* visited = NULL;
bool* notPermited = NULL;
int* tier1 = NULL;

int ASesNumber = 0;

int main(int argc, char *argv[]) 
{
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

	Heap* h = setupDijkstra(graph);

	//for (int i = MAXSIZE; i >= MAXSIZE-ITERATIONS; --i)
	for (int i = 0; i <= ITERATIONS; ++i) 
		if (tier1[i] > 0)
			GenDijkstra(graph, h, i);
		#ifdef COMMERCIAL
			providers += ASesNumber - 1 - (peers + customers);
		#endif

	clock_t end = clock();
	double time_spent1 = (double)(end - begin) / CLOCKS_PER_SEC;

	/********** SHORTEST PATHS **********/

	begin = clock();
	lengthShortestPaths(graph);

	end = clock();
	double time_spent2 = (double)(end - begin) / CLOCKS_PER_SEC;
	
	/********** FREE MEMORY **********/

	free(wt); free(st);
	free(lastcost);
	FreeHeap(h);
	freeGraph(graph);

	/********** PRINTS **********/

	printf("\n---------- COMMERCIAL PATHS ----------\n");

	// This is just to not count with the hops
	// that are 0, so corresponding to the start 
	// vertex in each iteration of the dijkstra
	totalHops[0] = 0;

	int counter = 0;
    for (int i = 1; i < ITERATIONS; ++i) {
    	if (totalHops[i] == 0)
    		break;
        counter += totalHops[i];
    }

    printf("counter %d\n", counter);

    int sum = 0;
    for (int i = 1; i < ITERATIONS; ++i) {
    	sum += totalHops[i-1];
    	printf("P(X>=%d) = %f\n", i, ((double)(counter - sum)/counter)*100);
    	if (!totalHops[i])
    		break;
    }
    
	printf("\n---------- COMMERCIAL ROUTE ELECTED ----------\n");
	
	printf("\n-> Total\n\n");

	//printf("total ASes: %d\n", ASesNumber);
	printf("providers: %d\n", providers);
	printf("peers: %d\n", peers);
	printf("customers: %d\n", customers);
	int totalRoutes = providers + peers + customers;
	printf("total routes: %d\n", totalRoutes);

	printf("\n-> Percentage\n\n");

	printf("providers route: %f\n", ((double)providers/totalRoutes)*100);
	printf("peers route: %f\n", ((double)peers/totalRoutes)*100);
	printf("customers route: %f\n", ((double)customers/totalRoutes)*100);

	printf("\n");
	printf("Total elapsed time for dijkstra: %f seconds\n\n", time_spent1);
	printf("Total elapsed time: %f seconds\n\n", time_spent1 + time_spent2);

	printf("Elapsed time in minutes would be for Dijkstra       (est) = %f\n", ((double)(MAXSIZE*(time_spent1))/ITERATIONS)/60);
	printf("Elapsed time in minutes would be for BFS            (est) = %f\n", ((double)(MAXSIZE*(time_spent2))/ITERATIONS)/60);
	printf("Elapsed time in minutes would be in total           (est) = %f\n", ((double)(MAXSIZE*(time_spent1+time_spent2))/ITERATIONS)/60);

	return 0;  
}

void print_usage(char *program)
{
	fprintf(stderr, "Usage: %s -f <input-file>\n", program);
	exit(1);
}