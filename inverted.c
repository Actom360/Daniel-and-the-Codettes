/*
reads data from a given collection of pages in collection.txt
and generates an "inverted index" that provides a list (set) of
urls for every word in a given collection of pages. 

You need to "normalise" words by removing leading and trailing spaces
and converting all characters to lowercases before inserting words in
your index. In each list (set), duplicate urls are not allowed.
Your program should output this "inverted index" to a file named invertedIndex.txt.
The list should be alphabetically ordered, using ascending order.
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "queue.h"
#include "graph.h"
#include "set.h"
#include "urlReader.h"

#define BUFSIZE 1024
 





//struct to tie "urlXX" file to set of words found in file
typedef struct IndexStruct {
   char*   url;    	// URL in "urlXX" form
   Set wordSet;		// Set of all words found in "urlXX.txt" body
} IndexStruct;


IndexStruct *newIndex(char *url){

	IndexStruct *i;
	i = malloc(sizeof(IndexStruct));

	i->url = url;
	i->wordSet = newSet();

	return i;
}





void printStringArr(char ** arr);
void fillWordSets(IndexStruct * indexArray, int n);
void fillStructArr(IndexStruct * indexArray, char **urlArr, int n);
Set combineWordSets(IndexStruct * indexArray, int n);



int main(int argc, char const *argv[])
{


	char *urlColl = getAllURLs();		//get all URLs in collection.txt
	int numURLTot = numWords(urlColl);	//get number of URLs
	char ** allURLs =  parseStringBySpaces(urlColl, numURLTot);	//get array of all URLs


	IndexStruct * wordsInURLs = malloc(numURLTot*sizeof(IndexStruct));	//initialize array to hold all IndexStructs

	fillStructArr(wordsInURLs, allURLs, numURLTot);		//fill array with IndexStructs constructed from the array of all URLs



	fillWordSets(wordsInURLs, numURLTot); //fill wordSets for each URL





	printStringArr(allURLs);

 
    return 0;
}


void fillStructArr(IndexStruct * indexArray, char **urlArr, int n)
{ 
	IndexStruct *ind;
	for (int i = 0; i < n; i++)
	{
		ind = newIndex(urlArr[i]);
		indexArray[i] = *ind;
	}

	printf("\n\ncleared fillStructArr\n\n");
}

//fills all wordSets in indexArray
//takes indexArray and size of array as parameters
void fillWordSets(IndexStruct * indexArray, int n)
{
	char url[6] = {0};
	char words[BUFSIZE];
	char **wordArr;
	int nWords;
	IndexStruct ind;

	for (int i = 0; i < n; i++)
	{

		ind = indexArray[i];

		

		strcpy(url, ind.url);
		printf("we made it thus far\n");
		strcpy(words, wordsInURL(url));

		nWords = numWords(words);

		wordArr =  parseStringBySpaces(words, nWords);

		for (int j = 0; j < nWords; j++)
		{
			insertInto(indexArray[i].wordSet, wordArr[j]);
		}


	}
	printf("\n\ncleared fillWordSets\n\n");

}



//takes already filled array of indices
//combines already filled sets into superset
Set combineWordSets(IndexStruct * indexArray, int n)
{
	Set s = newSet();

	char **wordArr;
	int setSize;
	IndexStruct ind;

	for (int i = 0; i < n; i++)
	{
		ind = indexArray[i];
		wordArr = getElemArr(ind.wordSet);
		setSize = nElems(ind.wordSet);

		for (int j = 0; j < setSize; j++)
		{
			insertInto(s, wordArr[j]);
		}

	}

	printf("\n\ncleared combineWordSets\n\n");


	return s;
}





void printStringArr(char ** arr) //for testing purposes
{
	int len = sizeof(arr)-1;

	for (int i = 0; i < len; i++)
	{
		printf("%s\n", arr[i]);
	}
	printf("\n\ncleared printStringArr\n\n");
}


