#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "queue.h"
#include "graph.h"
#include "urlReader.h"

#define BUFSIZE 1024
 

int main(int argc, char const *argv[])
{

		char *urlColl = getAllURLs();		//get all URLs in collection.txt
		int numURLTot = numURLs(urlColl);	//get number of URLs


		char ** allURLs =  parseStringBySpaces(urlColl);

		for (int i = 0; i < numURLTot; i++)
		{
			printf("%c\n", allURLs[i][0]);
		}
 
    return 0;
}


