/*
*		Normalized format for strings of words/urls is all words/urls
*		separated by a single space and all words completely lowercase
*
*/


char * getAllURLs();								//string of all URLs in collection.txt with spaces between urls
char ** parseStringBySpaces(char *allURLs);		//parses strings by spaces between words
int numURLs(char *allURLs);						//counts number of URLs in string
char * linksInURL(char * url);					//finds all links to other URLs in a single URL address and returns in normalized format
char * wordsInURL(char * url);					//fin

