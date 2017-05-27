#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE (80)
#define TRUE 1
#define FALSE 0




char* getAllURLs(int n) //store all URLs in collection.txt in one large String with spaces between 
{
	FILE *coll = fopen("collection.txt", "r"); 	//open file

    char line[BUFSIZE];							//var to hold line of text for loop below
    
    int len = 0, prevLen = 0, cnt = 0, addPos = 0; 			//counter variables
    int end;									//end of last url added, to properly finish full string

	assert(coll != NULL);  						//check file to read


	char *allURLs = malloc(150);				//to hold all URLs in one String
	char lastChar = 'a'; 						//to track last character added to 'allURLs'

	while(fgets(line, BUFSIZE, coll) != NULL) { //read File line by line, store in String 'line'

		len = strlen(line);
		cnt = 0;
		
		while(cnt < len){

			if(line[cnt] == '\n' || (lastChar == ' ' && line[cnt] == ' ')){ //if about to add newline or would cause more than one " " in allURLs
				cnt++;
				continue;
			}

			allURLs[addPos++] = line[cnt];		//add new char to allURLs
			lastChar = line[cnt];				//update lastChar added
			end = prevLen + cnt;
			cnt++;
		}

		prevLen += len;		//to keep allURLs from writing over itself when adding a new line

	}

	if(allURLs[end] != ' ') end++;		//increment to ensure not writing over url address


	allURLs[end] = '\0';				//finish string

	return allURLs;
}






char **parseStringBySpaces(char *string, int n)	//parses string and returns array of words contained
{
	char *tok;
	char *delim = " ";	//delimiter for parsing urls
	int cnt = 0;
	// int len = 0;
	char **urlArr = malloc(n * sizeof(char*));	//array to hold urls

 	
	tok = strtok(string, delim);
	for (int i = 0; i < n; i++)
	{
		urlArr[i] = malloc(6*sizeof(char));
	}

	// len = strlen(tok);
	// printf("parsing: %s  length: %d\n", tok, len);
	strcpy(urlArr[cnt++], tok);



	while((tok = strtok(NULL, delim)) != NULL) { //Iterate through to end of collection file, read line in coll and put inside String 'line'
		// len = strlen(tok);
		// printf("parsing: %s  length: %d\n", tok, len);
		strcpy(urlArr[cnt++], tok);
	} 

	// printf("\n\n" );


	// for (int i = 0; i < n; ++i)
	// {
	// 	printf("print urls: %s\n", 	urlArr[i]);
	// }

	return urlArr;
}






// int numURLs(char *allURLs)  //counts number of urls in string
// {

// 	int cnt = 0;
// 	int len = strlen(allURLs);

// 	for (int i = 0; i < len - 2; i++)
// 	{
// 		if(allURLs[i] == 'u' && allURLs[i+1] == 'r' && allURLs[i+2] == 'l') cnt++;
// 	}

// 	return cnt;
// }

int numWords(char *string)  //counts number of urls in string
{

	int cnt = 0;
	int len = strlen(string);
	int inWord = (string[0] != ' '); 			//Boolean to track if 'inside' word during iteration
	
	if(inWord) cnt++;

	char curChar;

	for (int i = 0; i < len; i++)
	{

		curChar = string[i];
		if (inWord)
		{
			if(curChar == ' '){
				inWord = FALSE;
			}

		}
		else if(string[i] != ' '){
				cnt++;
				inWord = TRUE;
		}
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

	int len = 0, cnt = 0, end = 0, addPos = 0;

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

				allURLs[addPos++] = line[cnt];	//add new char to allURLs
				lastChar = allURLs[addPos-1];	//update lastChar added
				end = cnt;
				cnt++;
			}		
		}
		
	}

	if(allURLs[end] != ' ') end++;		//increment to ensure not writing over url address

	allURLs[end] = '\0';				//finish string

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

	char *allURLs = malloc(1024);	//to hold all URLs in one String
	char lastChar = 'a'; 	//to track last character added to 'allURLs'

	int len = 0, cnt = 0, prevLen = 0, end = 0, addPos = 0;	//for filling new string
	int beginReading = FALSE;	//Boolean to track when to read for words

	while(fgets(line, BUFSIZE, f) != NULL) { //read File line by line, store in String 'line'
		

		cnt = 0;

		if (strstr(line, "Section-2") != NULL && beginReading == FALSE){ //stop reading lines after #end Section-1
			beginReading = TRUE;
			// printf("NOW READING FROM: %s\n", url);
			continue;
		} else if (strstr(line, "Section-2") != NULL){ //stop reading lines after #end Section-1
			beginReading = FALSE;
			break;
		}

		if (beginReading)
		{

			len = strlen(line);
			
			while(cnt < len){
				if(line[cnt] == '\n' || line[cnt] == '.' || (lastChar == ' ' && line[cnt] == ' ')){ //if about to add newline or would cause more than one " " in allURLs
					cnt++;
					continue;
				}

				allURLs[addPos++] = tolower(line[cnt]);	//add new char to allURLs
				lastChar = allURLs[addPos-1];				//update lastChar added
				end = prevLen + cnt;
				cnt++;
			}	

			prevLen += len;		//to keep allURLs from writing over itself when adding a new line

		}
	}

	if(allURLs[end] != ' ') end++;		//increment to ensure not writing over url address

	allURLs[end] = '\0';				//finish string

	return allURLs;

}

