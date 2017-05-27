#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "queue.h"

#define BUFSIZE 1024


void getAllUrls();

char* getAllURLs() //store all URLs in collection.txt in one large String with spaces between 
{
	FILE *coll = fopen("collection.txt", "r"); //open file

    char line[BUFSIZE];	//var to hold line of text for loop below
    
    int len = 0, prevLen = 0, cnt = 0; //counter variables
    int end; //end of last url added, to properly finish full string

	assert(coll != NULL);  //check file to read

	char allURLs[BUFSIZE] = {0};	//to hold all URLs in one String
	char lastChar = "a"; 	//to track last character added to 'allURLs'

	while(fgets(line, BUFSIZE, coll) != NULL) { //read File line by line, store in String 'line'
		
		prevLen += len;		//to keep allURLs from writing over itself when adding a new line
		len = strlen(line);
		
		while(cnt < len){
			if(line[cnt] == "\n" || lastChar == " "){ //if about to add newline or would cause more than one " " in allURLs
				cnt++;
				continue;
			}

			allURLs[prevLen+cnt] = line[cnt];	//add new char to allURLs
			lastChar = allURLs[cnt];				//update lastChar added
			end = cnt;
			cnt++;
		}
	}

	if(allURLs[end] != " ") end++;		//increment to ensure not writing over url address

	allURLs[end] = "\0";				//finish string

	printf("%s\n", allURLs); 
	return allURLs;
}






char **parseURLs(char *allURLs, int n)	//parses string and returns array of URLs (set)
{
	char *url;
	char *delim = " ";	//delimiter for parsing urls
	int cnt = 0;
	char urlArr[n][8];	//array to hold urls

	while((url = strtok(allURLs, delim)) != NULL) { //Iterate through to end of collection file, read line in coll and put inside String 'line'
		len = strlen(url);
		printf("parsing: %s  length: %d\n", url, len);

		strcpy(urlArr[cnt], url);
		cnt++;	
	} 
}






int numURLs(char *allURLs)  //counts number of urls
{

	int cnt = 0;
	int len = strlen(allURLs);

	for (int i = 0; i < len - 2; i++)
	{
		if(allURLs[i] == 'u' && allURLs[i+1] == 'r' && allURLs[i+2] == 'l') cnt++;
	}

	return cnt;
}


//store all links in specified URL page in one String with spaces between URLs
//NOTE: 'url' should be passed as "urlXX" not "urlXX.txt"
char * linksInURL(char * url, int num)	
{
	int len = strlen(url)
	char urlFile[len+4];
	
	for (int i = 0; i < len; i++)
	{
		urlFile[i] = url[i];
	}

	urlFile[len+1] = '.';
	urlFile[len+2] = 't';
	urlFile[len+3] = 'x';
	urlFile[len+4] = 't';

	char linksArr [num][BUFSIZE];
}




