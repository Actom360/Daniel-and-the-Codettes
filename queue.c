// Pqueue.c ... simple PQueue of Strings
// Written by John Shepherd, September 2015

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "pqueue.h"

typedef struct Node *Link;

typedef struct Node {
	char *val;
	Link  next;
} Node;
	
typedef struct PQueueRep {
	Link  front;
	Link  back;
} PQueueRep;

// Function signatures

PQueue newPQueue();
void disposePQueue(PQueue);
void enterPQueue(PQueue,char *);
char *leavePQueue(PQueue);
int  emptyPQueue(PQueue);
void showPQueue(PQueue q);

static Link newNode(char *);
static void disposeNode(Link);


// newPQueue()
// - create an initially empty PQueue
PQueue newPQueue()
{
	PQueue new = malloc(sizeof(PQueueRep));
	assert(new != NULL);
	new->front = NULL;
	new->back = NULL;
	return new;
}

// disposePQueue(PQueue)
// - clean up memory associated with PQueue
void disposePQueue(PQueue q)
{
	if (q == NULL) return;
	Link next, curr = q->front;
	while (curr != NULL) {
		next = curr->next;
		disposeNode(curr);	
		curr = next;
	}
}

// enterPQueue(PQueue,Str)
// - add Str to back of PQueue
void enterPQueue(PQueue q, char *str)
{
	Node cur = q->back;
	Node prev;

	Link new = newNode(str);
	if (q->front == NULL)
		q->front = q->back = new;
	else {

		//find place to add
		if(strcmp(str, cur->val) < 0){
			while(strcmp(str,cur->val) < 0 && cur->next != NULL) {
				prev = cur;
				cur = cur->next; 
			}

			new->next = cur;
			prev->next = new;
		} else {
			// add to back
			q->back->next = new;
			q->back = new;
		}
	}
}

// leavePQueue(PQueue)
// - return string at front of PQueue
char *leavePQueue(PQueue q)
{
	assert (q->front != NULL);
    char *str = q->front->val;
	Link old = q->front;
	q->front = old->next;
	if (q->front == NULL)
		q->back = NULL;
	free(old);
	return str;
}

// emptyPQueue(PQueue)
// - true if PQueue is empty
int emptyPQueue(PQueue q)
{
	return (q->front == NULL);
}

// showPQueue(PQueue)
// - display PQueue (for debugging)
void showPQueue(PQueue q)
{
	Link curr;
	if (q->front == NULL)
		printf("PQueue is empty\n");
	else {
		printf("PQueue (front-to-back):\n");
		int id = 0;
		curr = q->front;
		while (curr != NULL) {
			printf("[%03d] %s\n", id, curr->val);
			id++;
			curr = curr->next;
		}
	}
}

// Helper functions

static Link newNode(char *str)
{
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->val = strdup(str);
	new->next = NULL;
	return new;
}

static void disposeNode(Link curr)
{
	assert(curr != NULL);
	free(curr->val);
	free(curr);
}

