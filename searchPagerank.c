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


void printStringArr(char ** arr, int n);
Set getURLSet(char *string);
char **getURLArr(char *string);
char * urlsWithWord(char * string);
int needRemoveURL(char *url, Set rURL, Set tSet);
char *findPRLine(char *url);
float findPRVal(char *url);
int findHighRem(int *seen, float *vals, int num);
void printResults(char **allURL, float * prV, int num);






//	Because only using URLs that have all words searched for
//	we will start with set of URLs containing 1st word
//	and iteratively trim the set to meet this condition
int main(int argc, char const *argv[])
{
	int nWords = argc-1;
	char **wordsSearched = malloc(nWords * sizeof(char *));		//array of words to look for

	for (int i = 1; i <= nWords; i++)							//construct array of words that were searched
	{
		int len = strlen(argv[i]);
		wordsSearched[i-1] = malloc(len * sizeof(char));
		strcpy(wordsSearched[i-1], argv[i]);
	}



	char *curWord = malloc(15);					//var to iterate through words searched for				
	strcpy(curWord, wordsSearched[0]);			//start with very first word searched

	Set relURLs = getURLSet(curWord);				//set of URLs containing first word
	char **firstURLs = getURLArr(curWord);		//array of URLs containing first word
	int nURLs = nElems(relURLs);

	// printf("Printing Base RelURLS\n");
	// showSet(relURLs);


	Set curWordSet;								//Set to track urls tied to 'curWord'
	for (int i = 1; i < nWords; i++)			//iterate through words 2-n and remove any URLs that don't meet requirement
	{
		strcpy(curWord, wordsSearched[i]);		
		curWordSet = getURLSet(curWord);
		// printf("Printing %s URL Set\n", curWord);

		// showSet(curWordSet);

		for (int x = 0; x < nURLs; x++)			//iterate all URLs attached to 'curWord'
		{
			char *url = malloc(10);
			strcpy(url,firstURLs[x]);
			if(needRemoveURL(url, relURLs, curWordSet)){	//if not in common between all words
				dropFrom(relURLs, url);
			}
		}
	}


	// printf("Printing Final RelURLS\n");

	// showSet(relURLs);

	int numFinURLs = nElems(relURLs);
	char **finalURLs = malloc(numFinURLs * sizeof(char *));
	float *prVals = malloc(numFinURLs * sizeof(float));


	for (int i = 0; i < numFinURLs; i++)
	{
		strcpy(curWord, popString(relURLs));
		finalURLs[i] = malloc(strlen(curWord) +1);
		strcpy(finalURLs[i], curWord);
		prVals[i] = findPRVal(curWord);
	}

	printResults(finalURLs, prVals, numFinURLs);


	return 0;
}





//
//helper functions below
//

//prints final information
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

	int ins = 0;	//to hold position of highest PR value not already added to sortedURLs

	int cnt = 0;
	while(cnt < num)	//fill list sortedURLs with URLs in descending PR order
	{
		ins = findHighRem(seen, prV, num);	//get highest remaining PR
		seen[ins] = 1;						//add index "ins" to seen list
		sortedURLs[cnt] = ins;				//mark index "ins" as next highest PR
		cnt++;
	}


	int nextURL = sortedURLs[0];
	for (int i = 0; (i < num && i < 10); i++)
	{
		nextURL = sortedURLs[i];					//index of ith highest PR value
		printf("%s\n", allURL[nextURL]);			//print all wanted data to stdout
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

float findPRVal(char *url)
{
	char *info = findPRLine(url);
	char *tok;
	char *delim = ",";

	if(strcmp(info, " ") == 0) return 0.0;

	tok = strtok(info, delim);
	if (strstr(tok, "0.") != NULL)
	{
		return atof(tok);
	} else {
		while((tok = strtok(NULL, delim)) != NULL){
		 	if (strstr(tok, "0.") != NULL) return atof(tok);
		 }
	}

	// printf("to the end\n");
	return 0.0;

}


char *findPRLine(char *url)
{
	FILE * f = fopen("pagerankList.txt", "r");		//open Inverted Index file

    char line[BUFSIZE];	//var to hold line of text for loop below
    
	if(f == NULL){		//check file to read is valid (can't do anything without pagerankList.txt)
		printf("You Playing with Me\n");
		return "";
	}

	char *prInfo = malloc(BUFSIZE);	//to hold all URLs in one String
	char lastChar = 'a'; 	//to track last character added to 'prInfo'

	int len = 0, cnt = 0, addPos = 0;

	while(fgets(line, BUFSIZE, f) != NULL) { 	//read File line by line, store in String 'line'
		
		if (strstr(line, url) != NULL && strlen(url) >= 5) //read line containing 'url'
		{
			// printf("%s contains %s\n", line, string);

			len = strlen(line);
			cnt = 0;
			
			while(cnt < len){

				
				if(line[cnt] == '\n'){			//about to add newline or
					prInfo[addPos++] = ' ';	//add space to prInfo
					lastChar = prInfo[addPos-1];	//update lastChar added
					cnt++;	
					continue;	
				}

				if(lastChar == ' ' && line[cnt] == ' '){ //if would cause more than one " " in prInfo
					cnt++;
					continue;
				}

				prInfo[addPos++] = line[cnt];	//add new char to prInfo
				lastChar = prInfo[addPos-1];	//update lastChar added
			
				cnt++;

			}
			break;	
		}
		
	}

	if(strcmp(prInfo, "") == 0){
		return " ";
	}

	if(prInfo[addPos] != ' ') addPos++;		//increment to ensure not writing over info

	prInfo[addPos] = '\0';				//finish string

	return prInfo;						//returns pagerank info of url
}


//takes a URL as parameter and returns whether
//should remove from relURLs
int needRemoveURL(char *url, Set rURL, Set tSet)
{
	return (isElem(rURL, url) && !isElem(tSet, url));
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



void printStringArr(char ** arr, int n) 
{

	printf("\n\nTesting: %d\n", n);

	for (int i = 0; i < n; i++)
	{
		printf("%s\n", arr[i]);
	}
	printf("\n");
}