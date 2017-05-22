#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "queue.h"
#include "graph.h"

#define BUFSIZE 1024
 
void getAllUrls(Graph g, int nV);
int countNumURLs();
void fillAdjMatrix();


int main(int argc, char const *argv[])
{
	int numURLs = countNumURLs();
	Graph g = newGraph(numURLs);
	getAllUrls(g, numURLs);
 
    return 0;
}

void getAllUrls(Graph g, int nV)
{
	char *url;
	FILE *coll = fopen("collection.txt", "r");
    char line[BUFSIZE];

	assert(coll != NULL);	

	char *delim = " ";
	while(fgets(line, BUFSIZE, coll) != NULL) { //Iterate through to end of collection file, read line in coll and put inside String 'line'
		
		url = strtok(line, delim);

		if(strlen(url) == 5){
			printf("Adding: %s to position %d\n", url, getURLCount(g));
			addURL(g, url);
			printURLs(g);
		}

		while((url = strtok(NULL, delim)) != NULL){


			if(strlen(url) == 5){
				printf("Adding: %s to position %d\n", url, getURLCount(g));

				addURL(g, url);
				printURLs(g);
			}
		}
	} 
}

int countNumURLs(){
	FILE *coll = fopen("collection.txt", "r");
    char line[BUFSIZE];
    char * url;	
    char delim[2] = " ";
    int cnt = 0;
    while(fgets(line, BUFSIZE, coll) != NULL){
        
        // printf("%s\n", line);
        url = strtok(line, delim);
		if(strlen(url) > 3) cnt++;

		while((url = strtok(NULL, delim)) != NULL){
			if(strlen(url) > 3) cnt++;
		}
    }
    return cnt;
}

void fillAdjMatrix(){}

