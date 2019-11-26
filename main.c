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

	//the 1 added to MAXSIZE is for a bug correction in the dijkstra main cicle (this way the heap will always have a node with maxWT)
	struct Graph* graph = createGraph(MAXSIZE+1);

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


	int vertexes = printGraph(graph);
	printf("\n");

	int exploredNodes = GenDijkstra(graph, NULL, 4);

	for (int i =1, j=0; i <= 100; ++i)
		if (graph->tier1[i] > 0 && GenDijkstra(graph, NULL, i) != vertexes)
			printf("HEEEYYYpara i=%d, já vai em %d\n", i, ++j);
	


	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("Elapsed time: %f seconds\n\n", time_spent);
	printf("There are %d ASes\n", vertexes);
	printf("Dijkstra explored %d nodes\n\n", exploredNodes);
	
	freeGraph(graph);

	return 0;  
}

void print_usage(char *program)
{
	fprintf(stderr, "Usage: %s -f <input-file>\n", program);
	exit(1);
}