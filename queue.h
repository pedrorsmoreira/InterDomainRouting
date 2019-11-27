/* Prevent multiple inclusions                                      */
#ifndef QUEUE
#define QUEUE

#include <stdbool.h>

#include "defs.h"

struct queue
{
    int items[MAXSIZE];
    int front;
    int rear;
};

struct queue* createQueue();
bool isEmpty(struct queue* q);
void pushQueue(struct queue* q, int value);
int popQueue(struct queue* q);
void printQueue(struct queue *q);

/* End if: Protect multiple inclusions                              */
#endif