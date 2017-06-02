// searchTfIdf.c
// gcc searchTfIdf.c -o searchTfIdf

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "set.h"
#include "urlReader.h"
#include "math.h"

#define TRUE 1
#define FALSE 0
#define BUFSIZE 1024



typedef struct WordStruct {

	char*	word;
	char** 	urls;
	int 	nUrls;
	float*	tfVal;
	float	idfVal;	

} WordStruct;


WordStruct *newWordStruct(char* word) {

	WordStruct *w;
	w = malloc(sizeof(WordStruct));

	w->word = word;
	w->urls = NULL;
	w->nUrls = 0;
	w->tfVal = NULL;
	w->idfVal = 0.0;

	return w;
}



/* function prototype declarations */
Set getURLSet(char *string);
char **getURLArr(char *string);
char * urlsWithWord(char * string);
int needRemoveURL(char *url, Set rURL, Set tSet);
void printStringArr(char ** arr, int n);
void fillURLs(WordStruct *w);
int countWordInURL(char ** array, char* word, int size);
void calcTF(WordStruct *w);
void calcIDF(WordStruct *w, int  numURLTot, int numUrls);
Set getFinURLSet(WordStruct * wsArray, int n);
int findURLPos(char * url, char **allURLs, int numURLs);
float calcTFIDF(WordStruct *wArr, int nWords, char *url);
int findHighRem(int *seen, float *vals, int num);
void printResults(char **allURL, float * prV, int num);






int main(int argc, char const *argv[])
{
	if(argc < 2){
		printf("Usage: %s <Space separated words>\n", argv[0]);
		exit(0);
    }

    int nWords = argc-1;
	char **wordsSearched = malloc(nWords * sizeof(char *));		// array of words to look for

    // read the command line arguments
    if (argc > 1) {
    	int i;
    	for (i = 1; i <= nWords; i++) {

    		// put words in array
    		int len = strlen(argv[i]);
    		wordsSearched[i-1] = malloc(len * sizeof(char));
    		strcpy(wordsSearched[i-1], argv[i]);

    		// all words are in the wordsSearched array now

    		for (int i = 0; i < wordsSearched; ++i)
    		{
    			// check if word exists in urls (loop through wordsSearched)
    				// delete from array
    				// if not, decrement nWords(!)

    			// check if array is not empty after removing element, if so return 0;
    			// and print something like "no matching urls found"
    		}

    		// .. could also filter this* out before even putting it into wordsSearched
    		// *the words that are not in any url
    		
    	}
    }

    WordStruct * wordsArray = malloc(nWords*sizeof(WordStruct));

    int numURLTot = numWords(getAllURLs());

    for (int i = 0; i < nWords; ++i)
    {
    	wordsArray[i] = *newWordStruct(wordsSearched[i]);

    	fillURLs(&wordsArray[i]);
    	// printStringArr(wordsArray[i].urls, wordsArray[i].nUrls);

    }

    Set urlSet = getFinURLSet(wordsArray, nWords);
    int numFinURL = nElems(urlSet);

    char **finalURLs = malloc(numFinURL * sizeof(char *));
    finalURLs = getElemArr(urlSet);

	float *tfidfVals = malloc(numFinURL * sizeof(float));

	for (int i = 0; i < nWords; ++i)
    {

    	calcTF(&wordsArray[i]);
    	calcIDF(&wordsArray[i], numURLTot, wordsArray[i].nUrls);
    }
    // printStringArr(finalURLs, numFinURL);

    for (int i = 0; i < numFinURL; ++i)
    {

    	tfidfVals[i] = calcTFIDF(wordsArray, nWords, finalURLs[i]);
    	// printf("%d, %d\n", i, numURLTot);
    }

    printResults(finalURLs, tfidfVals, numFinURL);

	return 0;

}


/* helper functions */
void printResults(char **allURL, float * prV, int num)
{
	int sortedURLs[num];
	int seen[num];

	//has to set all values to 0 because compiler not happy
	for (int i = 0; i < num; ++i)
	{
		sortedURLs[i] = 0;
		seen[i] = 0;
	}

	int ins = 0;		// to hold position of highest PR value not already added to sortedURLs

	int cnt = 0;
	while(cnt < num)	// fill list sortedURLs with URLs in descending PR order
	{
		ins = findHighRem(seen, prV, num);	// get highest remaining PR
		seen[ins] = 1;						// add index "ins" to seen list
		sortedURLs[cnt] = ins;				// mark index "ins" as next highest PR
		cnt++;
	}


	int nextURL = sortedURLs[0];
	for (int i = 0; (i < num && i < 10); i++)
	{
		nextURL = sortedURLs[i];					// index of ith highest PR value
		printf("%s, %.6f\n", allURL[nextURL], prV[nextURL]);			// print all wanted data to stdout
	}
}

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

//takes word to search for
//returns set of all URLs containing the word
Set getURLSet(char *string)
{
	Set s = newSet();

	char *urls = urlsWithWord(string);
	int n = numWords(urls);

	// printf("%s url set is %s\n", string, urls);
	char **urlArr = parseStringBySpaces(urls);

	// printf("%s Array Printed\n", string);
	// printStringArr(urlArr, n);
	char temp[6] = "";

	for (int i = 0; i < n; i++)
	{
		strcpy(temp, urlArr[i]);
		insertInto(s, temp);
	}

	return s;
}

//takes already filled array of indices
//combines already filled sets into superset
Set getFinURLSet(WordStruct * wsArray, int n)
{
	Set s = newSet();

	char **urlArr = malloc(n*sizeof(char *));
	WordStruct ws;


	for (int i = 0; i < n; i++)
	{
		ws = wsArray[i];
		urlArr = ws.urls;


		for (int j = 0; j < ws.nUrls; j++)
		{

			insertInto(s, urlArr[j]);
		}
		// printf("%d, %d\n",i, n );

	}

	return s;
}



//takes word to search for
//returns array of URLs containing the word
char **getURLArr(char *string)
{
	char *urls = urlsWithWord(string);
	char **urlAr = parseStringBySpaces(urls);


	return urlAr;
}


//takes word to search for
//returns string containing all URLs containing this string
char * urlsWithWord(char * string)
{
	FILE * f = fopen("invertedIndex.txt", "r");		//open Inverted Index file

    char line[BUFSIZE];	//var to hold line of text for loop below
    
	if(f == NULL){		//check file to read is valid (can't do anything without invertedIndex.txt)
		printf("You Playing with Me\n");
		return "";
	}

	char *allURLs = malloc(BUFSIZE);	//to hold all URLs in one String
	char lastChar = 'a'; 	//to track last character added to 'allURLs'

	int len = 0, cnt = 0, addPos = 0;

	int readURLs = FALSE;

	while(fgets(line, BUFSIZE, f) != NULL) { 	//read File line by line, store in String 'line'
		
		if (strstr(line, string) != NULL) //read line for 'string'
		{
			// printf("%s contains %s\n", line, string);

			len = strlen(line);
			cnt = 0;
			
			while(cnt < len){

				if(strstr(&line[cnt], string) == NULL){

					if(readURLs){
						if(line[cnt] == '\n'){			//about to add newline or
							allURLs[addPos++] = ' ';	//add space to allURLs
							lastChar = allURLs[addPos-1];	//update lastChar added
							cnt++;	
							continue;	
						}
		
						if(lastChar == ' ' && line[cnt] == ' '){ //if would cause more than one " " in allURLs
							cnt++;
							continue;
						}
		
						allURLs[addPos++] = line[cnt];	//add new char to allURLs
						lastChar = allURLs[addPos-1];	//update lastChar added
					}

					if(line[cnt] == ' ') readURLs = TRUE;

				}
				cnt++;

			}
			break;	
		}
		
	}

	if(allURLs[addPos] != ' ') addPos++;		//increment to ensure not writing over url address

	allURLs[addPos] = '\0';				//finish string

	return allURLs;						//returns normalized string of all URLs containing 'string'

}



// takes a URL as parameter and returns whether
// should remove from relURLs
int needRemoveURL(char *url, Set rURL, Set tSet)
{
	return (isElem(rURL, url) && !isElem(tSet, url));
}

void fillURLs(WordStruct *w)
{
	char* urls = urlsWithWord(w->word);
	int n = numWords(urls);
	char **urlArr = parseStringBySpaces(urls);

	w->nUrls = n;
	w->urls = urlArr;

}

void printStringArr(char ** arr, int n) 
{

	printf("\n\nTesting: %d\n", n);

	for (int i = 0; i < n; i++)
	{
		printf("%s\n", arr[i]);
	}
}


void calcTF(WordStruct *w)
{
	char* curURL = malloc(sizeof(char) * 6);

	w->tfVal = malloc(sizeof(float) * w->nUrls);

	for (int i = 0; i < w->nUrls; ++i)
	{
		strcpy(curURL, w->urls[i]);

		char* allWords = malloc(BUFSIZE);
		strcpy(allWords, wordsInURL(curURL));

		int n = numWords(allWords);

		char **wordsArr = parseStringBySpaces(allWords);
		int countOfWord = countWordInURL(wordsArr, w->word, n);

		float value = (float)countOfWord / n;
		w->tfVal[i] = value;

		// printf("tf value: %.6f\n", w->tfVal[i]);

	}

}

void calcIDF(WordStruct *w, int  numURLTot, int numUrls)
{
	float value = log10((float)numURLTot/numUrls);
	w->idfVal = value;

	// printf("idf value: %.6f\n", w->idfVal);

}

int countWordInURL(char ** array, char* word, int size)
{	
	int count = 0;

	for (int i = 0; i < size; ++i)
	{
		if(strcmp(array[i], word) == 0) {
			count++;
		}
	}

	return count;

}

float calcTFIDF(WordStruct *wArr, int nWords, char *url)
{
	WordStruct w;
	int pos = -1;
	float sum = 0;
	for (int i = 0; i < nWords; ++i)
	{
		w = wArr[i];
		pos = findURLPos(url, w.urls, w.nUrls);
		if(pos != -1){
			sum += w.tfVal[pos] * w.idfVal;
		}
	}
	return sum;

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

	return -1;
}