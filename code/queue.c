#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue* createQueue()
{
    struct queue* q = malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

bool isEmpty(struct queue* q)
{
    if(q->rear == -1) 
        return true;
    else 
        return false;
}

void pushQueue(struct queue* q, int value)
{
    if(q->rear == MAXSIZE-1)
        printf("Queue is Full\n");
    else {
        if(q->front == -1)
            q->front = 0;
        q->rear++;
        q->items[q->rear] = value;
    }
}

int popQueue(struct queue* q)
{
    int item;
    if(isEmpty(q)) {
        printf("Queue is empty");
        item = -1;
    } else {
        item = q->items[q->front];
        q->front++;
        if(q->front > q->rear){
            //printf("Reseting queue\n");
            q->front = q->rear = -1;
        }
    }
    return item;
}

void printQueue(struct queue *q)
{
    int i = q->front;
    if(isEmpty(q)) {
        printf("Queue is empty");
    } else {
        printf("\nQueue contains \n");
        for(i = q->front; i < q->rear + 1; i++)
            printf("%d ", q->items[i]);
        printf("\n");
    }    
}