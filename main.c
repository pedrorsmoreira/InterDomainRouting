#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


#include <time.h>

#include "graph.h"

void print_usage(char*);

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





#include <float.h>

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



	struct Graph* graph = readFile(filename);

	if (!checkCustomersCycles(graph))
		printf("HAVE CUSTOMER CYCLES\n");

	if (!checkCommercialConnectedness(graph))
		printf("NOT COMMERCIAL CONNECTED\n");


	//int vertexes = printGraph(graph);
	//printf("\n");


	//GET READY FOR DIJKSTRA
	wt = (int *) malloc(MAXSIZE * sizeof(int));
    st = (bool *) malloc(MAXSIZE * sizeof(bool));
    lastcost = (unsigned int *) malloc(MAXSIZE * sizeof(unsigned int));
	
    int ASesNumber=0;
	for (int i = 0; i < graph->V; ++i)
		if (graph->tier1[i] > 0)
            ++ASesNumber;

	Heap* h = NewHeap(ASesNumber+1, MAXSIZE, LessNum);
	int *HeapPositions = getHeapElementes_pos(h);

	for (int i = 0, j = 0; i < graph->V; ++i) {
		if (graph->tier1[i] == 0 && i > 0){
			HeapPositions[i] = i;
            continue;
        }
        int *v = (int *) malloc(sizeof(int)); //fazer isto fora e depois mudar só e dar os "free(v)" só no fim
        if (v == NULL) printf("failed malloc\n");;
        
        *v = i;
        Direct_Insert(h, (Item) v);
        HeapPositions[i] = j++;
    }printf("SETUP DONE\n");
    //READY FOR DIJKSTRA

    int exploredtotal = 0;
	for (int i =1, j=0; i <= 100; ++i) {
	//for (int i =1, j=0; i <= MAXSIZE; ++i) {
		if (graph->tier1[i] > 0)
			exploredtotal+=GenDijkstra(graph, h, i);
			//printf("------------DIJKSTRA%d\n", GenDijkstra(graph, h, i));

		//printf("\ni: %d\n", i);
	}
	
	//free memory alloced for dijkstra
	free(wt); free(st);
	free(lastcost);
	FreeHeap(h);

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("Elapsed time: %f seconds\n\n", time_spent);
	//printf("There are %d ASes\n", vertexes);
	//printf("Dijkstra explored %d nodes\n\n", exploredNodes);
	printf("Dijkstra explored %d nodes\n\n", exploredtotal);

	freeGraph(graph);

	return 0;  
}

void print_usage(char *program)
{
	fprintf(stderr, "Usage: %s -f <input-file>\n", program);
	exit(1);
}