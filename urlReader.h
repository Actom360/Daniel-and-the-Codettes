/*
*		Normalized format for strings of words/urls is all words/urls
*		separated by a single space and all words completely lowercase
*
*/


char * getAllURLs();								//string of all URLs in collection.txt with spaces between urls
char ** parseStringBySpaces(char *allURLs, int);		//parses strings by spaces between words given n number of words
int numWords(char *string);						//counts number of words/URLs in string
char * linksInURL(char * url);					//finds all links to other URLs in a single URL address and returns in normalized format
char * wordsInURL(char * url);					//finds all words in URL file and returns in normalized format




// char * makeURLFile(char * url);					//takes string form "urlXX" converts to "urlXX.txt"

