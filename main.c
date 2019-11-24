#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <string.h>

#include "graph.h"

void print_usage(char*);


struct Graph* readFile(char* filename)
{
	FILE *fp = fopen (filename, "r");
	if (fp == NULL) {
		perror (filename);
		exit(1);
	}

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

		addEdge(graph, atoi(token), atoi(token2)); 
	}

	fclose (fp);

	return graph;
}

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

	struct Graph* graph = readFile(filename);

	printGraph(graph);

	return 0;  
}

void print_usage(char *program)
{
	fprintf(stderr, "Usage: %s -f <input-file>\n", program);
	exit(1);
}