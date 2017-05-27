#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 1024
#define TRUE 1
#define FALSE 0




char* getAllURLs(int n) //store all URLs in collection.txt in one large String with spaces between 
{
	FILE *coll = fopen("collection.txt", "r"); 	//open file

    char line[BUFSIZE];							//var to hold line of text for loop below
    
    int len = 0, prevLen = 0, cnt = 0; 			//counter variables
    int end;									//end of last url added, to properly finish full string

	assert(coll != NULL);  						//check file to read

	char *allURLs = malloc(BUFSIZE);				//to hold all URLs in one String
	char lastChar = 'a'; 						//to track last character added to 'allURLs'

	while(fgets(line, BUFSIZE, coll) != NULL) { //read File line by line, store in String 'line'
		
		len = strlen(line);
		
		while(cnt < len){
			if(line[cnt] == '\n' || lastChar == ' '){ //if about to add newline or would cause more than one " " in allURLs
				cnt++;
				continue;
			}

			allURLs[prevLen+cnt] = line[cnt];		//add new char to allURLs
			lastChar = allURLs[cnt];				//update lastChar added
			end = prevLen + cnt;
			cnt++;
		}

		prevLen += len;		//to keep allURLs from writing over itself when adding a new line

	}

	if(allURLs[end] != ' ') end++;		//increment to ensure not writing over url address

	allURLs[end] = '\0';				//finish string

	printf("%s\n", allURLs); 
	return allURLs;
}






char **parseStringBySpaces(char *allURLs, int n)	//parses string and returns array of words contained
{
	char *url;
	char *delim = " ";	//delimiter for parsing urls
	int cnt = 0, len = 0;
	char **urlArr = calloc(n, sizeof(char*));	//array to hold urls
	for (int i = 0; i < n; i++)
	{
		urlArr[i] = malloc(12*sizeof(char));
	}

	while((url = strtok(allURLs, delim)) != NULL) { //Iterate through to end of collection file, read line in coll and put inside String 'line'
		len = strlen(url);
		printf("parsing: %s  length: %d\n", url, len);

		strcpy(urlArr[cnt], url);
		cnt++;	
	} 

	return urlArr;
}






int numURLs(char *allURLs)  //counts number of urls in string
{

	int cnt = 0;
	int len = strlen(allURLs);

	for (int i = 0; i < len - 2; i++)
	{
		if(allURLs[i] == 'u' && allURLs[i+1] == 'r' && allURLs[i+2] == 'l') cnt++;
	}

	return cnt;
}


char * makeURLFile(char * url) //takes string form "urlXX" converts to "urlXX.txt"
{

	int len = strlen(url);
	char *urlFile = calloc(len+4, sizeof(char));
	
	for (int i = 0; i < len; i++)
	{
		urlFile[i] = url[i];
	}

	urlFile[len] = '.';
	urlFile[len+1] = 't';
	urlFile[len+2] = 'x';
	urlFile[len+3] = 't';

	return urlFile;
}


//store all links in specified URL page in one String with spaces between URLs
//NOTE: 'url' should be passed as "urlXX" not "urlXX.txt"
char * linksInURL(char * url)
{
	char* txtFile = makeURLFile(url);	// convert to "urlXX.txt"
	FILE * f = fopen(txtFile, "r");		//open file

    char line[BUFSIZE];	//var to hold line of text for loop below
    
	assert(f != NULL);  //check file to read

	char *allURLs = malloc(BUFSIZE);	//to hold all URLs in one String
	char lastChar = 'a'; 	//to track last character added to 'allURLs'

	int len = 0, cnt = 0, end = 0;

	while(fgets(line, BUFSIZE, f) != NULL) { 	//read File line by line, store in String 'line'
		
		if (strstr(line, "#end") != NULL) break;	//stop reading lines after #end Section-1


		if (strstr(line, "url") != NULL)
		{
			len = strlen(line);
			
			while(cnt < len){
				if(line[cnt] == '\n' || lastChar == ' '){ //if about to add newline or would cause more than one " " in allURLs
					cnt++;
					continue;
				}

				allURLs[cnt] = line[cnt];	//add new char to allURLs
				lastChar = allURLs[cnt];				//update lastChar added
				end = cnt;
				cnt++;
			}		
		}
		
	}

	if(allURLs[end] != ' ') end++;		//increment to ensure not writing over url address

	allURLs[end] = '\0';				//finish string

	printf("%s\n", allURLs); 
	return allURLs;

}


//store all words in specified URL page in one String with spaces between URLs
//NOTE: 'url' should be passed as "urlXX" not "urlXX.txt"
char * wordsInURL(char * url)
{
	char* txtFile = makeURLFile(url);	// convert to "urlXX.txt"
	FILE * f = fopen(txtFile, "r");		//open file

    char line[BUFSIZE];	//var to hold line of text for loop below
    
	assert(f != NULL);  //check file to read

	char *allURLs = malloc(BUFSIZE);	//to hold all URLs in one String
	char lastChar = 'a'; 	//to track last character added to 'allURLs'

	int len = 0, cnt = 0, prevLen = 0, end = 0;	//for filling new string
	int beginReading = 0;	//Boolean to track when to read for words

	while(fgets(line, BUFSIZE, f) != NULL) { //read File line by line, store in String 'line'
		
		if (strstr(line, "#start Section-2") != NULL){ //stop reading lines after #end Section-1
			beginReading = TRUE;
			continue;
		} else if (strstr(line, "#end Section-2") != NULL){ //stop reading lines after #end Section-1
			break;
		}

		if (beginReading)
		{

			len = strlen(line);
			
			while(cnt < len){
				if(line[cnt] == '\n' || lastChar == ' '){ //if about to add newline or would cause more than one " " in allURLs
					cnt++;
					continue;
				}

				allURLs[prevLen+cnt] = tolower(line[cnt]);	//add new char to allURLs
				lastChar = allURLs[cnt];				//update lastChar added
				end = prevLen + cnt;
				cnt++;
			}	

			prevLen += len;		//to keep allURLs from writing over itself when adding a new line

		}
	}

	if(allURLs[end] != ' ') end++;		//increment to ensure not writing over url address

	allURLs[end] = '\0';				//finish string

	printf("%s\n", allURLs); 
	return allURLs;

}

