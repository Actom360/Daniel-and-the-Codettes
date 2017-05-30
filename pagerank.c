#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "queue.h"
#include "graph.h"
#include "set.h"
#include "urlReader.h"
#include "math.h"











void printStringArr(char ** arr, int n);
int getNumStrInArr(char ** arr);
int findURLPos(char * url, char **allURLs, int numURLs);
void calcPRs(Graph g, float * prVals, float *prVals1, float d, int n, int *outArr);
void fillGraphEdges(Graph g, char ** allURLs, int n);
void fillOutArr(char **allURLs, int numURLTot, int *outArr);
int findHighRem(int *seen, float *vals, int num);
void printResults(char **allURL, int *outA, float * prV, int num);






	
int main(int argc, char const *argv[])
{

	assert(argc == 4);


	char *urlColl = getAllURLs();		//get all URLs in collection.txt
	strcpy(urlColl, getValidURLs(urlColl));

	printf("%s\n", urlColl);
	
	int numURLTot = numWords(urlColl);	//get number of URLs
	printf("there are %d URLs\n", numURLTot);

	if (numURLTot == 0)
	{
		printf("There is nothing to read. Ending program\n");
		return 0;	
	}


	char ** allURLs =  parseStringBySpaces(urlColl);	//make array of all urls
	int outArr[numURLTot];
	fillOutArr(allURLs, numURLTot, outArr);

	float d = atof(argv[1]);
	float diffPR = atof(argv[2]);
	int maxIter = atoi(argv[3]);





	//arrays to hold pr values
	float *prVals = malloc(numURLTot * sizeof(float));
	float *prVals1 = malloc(numURLTot * sizeof(float)); 



	for (int i = 0; i < numURLTot; i++)		//initialize PR arrays to proper value
	{
	
		prVals[i] = ((float)(1))/numURLTot;
		prVals1[i] = ((float)(1))/numURLTot;
	}

	Graph g = newGraph(numURLTot);
	fillGraphEdges(g, allURLs, numURLTot);

	//showGraph(g, allURLs); //was for testing




	float diff = diffPR;
	int it = 0;
	while(diff 	>= diffPR && it < maxIter){
		it++;
		calcPRs(g, prVals, prVals1, d, numURLTot, outArr);	
		diff = 0.0;

		for (int i = 0; i < numURLTot; ++i)
		{
			diff += fabs(prVals[i] - prVals1[i]);

			prVals[i] = prVals1[i];
		}

	}



	printResults(allURLs, outArr, prVals1, numURLTot);
 
    return 0;
}


void printResults(char **allURL, int *outA, float * prV, int num)
{
	int sortedURLs[num];
	int seen[num];

	//has to set all values to 0 because compiler not happy
	for (int i = 0; i < num; ++i)
	{
		sortedURLs[i] = 0;
		seen[i] = 0;
	}

	int ins = 0;	//to hold position of highest PR value not already added to sortedURLs

	int cnt = 0;
	while(cnt < num)	//fill list sortedURLs with URLs in descending PR order
	{
		ins = findHighRem(seen, prV, num);	//get highest remaining PR
		seen[ins] = 1;						//add index "ins" to seen list
		sortedURLs[cnt] = ins;				//mark index "ins" as next highest PR
		cnt++;
	}

	FILE *f = fopen("pagerankList1.txt", "w");		//file to write to

	int nextURL = sortedURLs[0];
	for (int i = 0; i < num; i++)
	{
		nextURL = sortedURLs[i];					//index of ith highest PR value
		fprintf(f, "%s, %d, %.8f\n", allURL[nextURL], outA[nextURL], prV[nextURL]);	//print all wanted data to file
	}


}

//finds index of highest PR value that hasn't been added to sorted array
// is for printing purposes
int findHighRem(int *seen, float *vals, int num)
{
	int pos = 0;
	for (int x = 0; x < num; x++)	//set pos to least val index
	{
		if(vals[x] < vals[pos]) pos = x;
	}
	for (int i = 0; i < num; i++)	//work pos up list to highest unused value
	{
		if(vals[i] >= vals[pos] && !seen[i]) pos = i;
	}

	return pos;
}

//fills array with number of outgoing links from URL
void fillOutArr(char **allURLs, int numURLTot, int *outArr)
{
	for (int i = 0; i < numURLTot; i++)
	{
		outArr[i] =  numWords(getValidURLs(linksInURL(allURLs[i])));
	}
}


//finds all outgoing links and makes edges on Graph
//takes Graph, array of all URLs, and total number of URLs
void fillGraphEdges(Graph g, char ** allURLs, int n)
{

	char *outLinks = linksInURL(allURLs[0]);
	strcpy(outLinks, linksInURL(allURLs[0]));
	strcpy(outLinks, getValidURLs(outLinks));



	char **outLinkArr = parseStringBySpaces(outLinks);

	int posTo;

	for (int i = 0; i < n; i++)
	{
		strcpy(outLinks, linksInURL(allURLs[i]));	//get all outgoing links from URL
		strcpy(outLinks, getValidURLs(outLinks));	//eliminate invalid links


		if(strcmp(outLinks, "") == 0) {printf("no outgoing links %s\n\n", outLinks); continue;}			//in case that there are no outgoing URLs

		int numURLs = numWords(outLinks);
		outLinkArr = parseStringBySpaces(outLinks);
		for (int x = 0; x < numURLs; x++)
		{
			posTo = findURLPos(outLinkArr[x], allURLs, n);		//find index val of URL
			insertEdge(g, i, posTo);							//make edge from i to posTo
		}
	}
}




//code for running iteration of PR calculation
void calcPRs(Graph g, float * prVals, float *prVals1, float d, int n, int *outArr)
{
	float sum = 0.0;

	for (int i = 0; i < n; i++){	//iterate through every URL
		
		sum = 0.0;
		for(int j = 0; j < n; j++){	//iterate through every URL again

			if(i != j){				//no self-looping links

				if(isEdge(g, j, i)){	//if there is a link from URL j to URL i

					sum +=  (prVals[j]/((float)outArr[j]));
				}
			}
		}

		float pr = (1.0-d)/((float)n);
		pr += d*sum;

		prVals1[i] = pr;
	}
}

//find the index in allURLs that url is found in
int findURLPos(char * url, char **allURLs, int numURLs)
{
	int pos;

	for (pos = 0; pos < numURLs; pos++)
	{
		if (strcmp(url, allURLs[pos]) == 0)
		{
			return pos;
		}
	}

	assert(pos < numURLs);
	return 0;
}



//prints any array of strings
//for testing purposes

void printStringArr(char ** arr, int n) 
{

	printf("\n\nTesting: %d\n", n);

	for (int i = 0; i < n; i++)
	{
		printf("%s\n", arr[i]);
	}
}

//returns number of columns in array of strings
//still buggy, did not use
// int getNumStrInArr(char ** arr)
// {
// 	int cnt = 0;
// 	while(arr[cnt] != NULL && strcmp(arr[cnt], "") != 0){
// 		cnt++;
// 	}

// 	return cnt-1;
// }


