/*
*		Normalized format for strings of words/urls is all words/urls
*		separated by a single space and all words completely lowercase
*
*/

//string of all URLs in collection.txt with spaces between urls
char * getAllURLs();								

//parses strings by spaces between words given n number of words
char ** parseStringBySpaces(char *allURLs, int);		

//counts number of words/URLs in string
int numWords(char *string);						

//finds all links to other URLs in a single URL address and returns in normalized format
//pass as "urlXX" not "urlXX.txt"
char * linksInURL(char * url);					

//finds all words in URL file and returns in normalized format
//pass as "urlXX" not "urlXX.txt"
char * wordsInURL(char * url);					



