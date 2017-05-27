#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "queue.h"
#include "graph.h"
#include "urlReader.h"

#define BUFSIZE 1024
 
void printStringArr(char ** arr);


int main(int argc, char const *argv[])
{

	char *urlColl = getAllURLs();		//get all URLs in collection.txt
	int numURLTot = numWords(urlColl);	//get number of URLs


	char ** allURLs =  parseStringBySpaces(urlColl, numURLTot);

	printStringArr(allURLs);

 
    return 0;
}





void printStringArr(char ** arr) //for testing purposes
{
	int len = sizeof(arr)-1;

	for (int i = 0; i < len; i++)
	{
		printf("%s\n", arr[i]);
	}
}


