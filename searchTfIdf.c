// searchTfIdf.c

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

// functions getting allURLs etc.


Set getURLSet(char *string);							/* function prototype declarations */
char **getURLArr(char *string);
char * urlsWithWord(char * string);

int main(int argc, char const *argv[])
{
	if(argc < 2){
		printf("Usage: %s <Space separated words>\n", argv[0]);
		exit(0);
    }

    int nWords = argc-1;
	char **wordsSearched = malloc(nWords * sizeof(char *));		//array of words to look for

	printf("Number of words: %i\n", nWords);


    // read the command line arguments
    if (argc > 1) {
    	int i;
    	printf("\n%s\n", "Command line arguments:");
    	for (i = 1; i <= nWords; i++)	{
    		printf("%i ", i);
    		printf("%s\n", argv[i]);

    		// put words in array
    		int len = strlen(argv[i]);
    		wordsSearched[i-1] = malloc(len * sizeof(char));
    		strcpy(wordsSearched[i-1], argv[i]);

    		printf("Inserting into array: %s\n", wordsSearched[i-1]);
    		
    	}
    }

    char *curWord = malloc(15);					//var to iterate through words searched for				
	strcpy(curWord, wordsSearched[0]);			//start with very first word searched

	Set relURLs = getURLSet(curWord);				//set of URLs containing first word
	char **firstURLs = getURLArr(curWord);		//array of URLs containing first word
	int nURLs = nElems(relURLs);

	printf("Printing Base RelURLS\n");
	showSet(relURLs);







    int numOfURLs = 0;

    // determine number of URLs
    FILE *collection = fopen("collection.txt", "r");
    // do something; count nr of URLs
    fclose(collection);

    // determine what URLs contain word
    collection = fopen("invertedIndex.txt", "r");


}

/* helper functions */

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




// gcc searchTfIdf.c -o searchTfIdf





//void getIDF





// calculate TF; how frequently a term occurs in a document

/*
what about the length of the document? could influence this;
to normalize this: 
TF(t) = (Number of times term t appears in a document) / (Total number of terms in the document).
*/

// calculate IDF; how important a term is

/*
- calculate total amount of documents
- calculate in how many documents the term appears

*/