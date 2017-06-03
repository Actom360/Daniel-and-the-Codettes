#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "set.h"
#include "urlReader.h"

#define TRUE 1
#define FALSE 0
#define BUFSIZE 1024

// int getRanks(char[], char*[][]);
int checkArgs(int);
void printStringArr(char ** arr, int n);
void fillAllURLSet(Set s, char**fileNames, int nFiles);
int findStringPos(char * string, char **strArr, int numStrings);
void fillNURLsInFile(int *nURLArr, char **fileNames, int nFiles);
char *getURLsInFile(char *fName);
void fillURLRanks(int **urlRanks, int nFiles, int nURLs, char **urlArr, char **fArr);
int getRank(char fileName[], char *url);
int fact(int n);
void lockURLs(int **urlRanks, int *lockedRanks, int nURLsTot, int nFiles);
int countNumLocks(int *lockedRanks, int nURLsTot);
void swap(int *fir, int *sec);
void permutation(int * vals, int curr, int size, int *locks, Set seen);
char *getPermString(int *arr, int size);
void fillSFRVals(float *sfrVals, int **pVals, int **urlRanks, int nURLRanked, int numPVals, int nFiles, int *nURLsInFile, char **fileNames, char **allURLs);
int findMinPos(float *sfrVals, int numPVals);
void printResults(int minPos, float *sfrVals, int **pVals, char **allURLArr, int nURLsTot);










int main(int argc, char *argv[]) {

    assert(checkArgs(argc));    // Checks the amount of command line arguments is appropriate
    int nFiles = argc-1;        //num of files to read
    char **fileNames = malloc(nFiles * sizeof(char*)); // stores names of rank files
    int *nURLsInFile = malloc(nFiles * sizeof(int));    // stores the number of rows in each column (i.e. how many urls are in each file)

    char temp[BUFSIZE];
    for (int i = 0; i < nFiles; ++i)            //fill fileNames array
    {
        strcpy(temp, argv[i+1]);
        fileNames[i] = malloc(strlen(temp) +1);
        strcpy(fileNames[i], temp);
    }


    fillNURLsInFile(nURLsInFile, fileNames, nFiles);



    Set allURLSet = newSet();            
    fillAllURLSet(allURLSet, fileNames, nFiles); //fill set with all possible URLs
    int nURLsTot = nElems(allURLSet);
    char **allURLArr = malloc(nURLsTot * sizeof(char *));
    allURLArr = getElemArr(allURLSet);

    int **urlRanks = malloc(nURLsTot* sizeof(int *));     //array to hold rank positions
    for (int i = 0; i < nURLsTot; ++i)
    {
        urlRanks[i] = malloc(nFiles * sizeof(int));
    }

    fillURLRanks(urlRanks, nFiles, nURLsTot, allURLArr, fileNames);
    int *lockedRanks = malloc(nURLsTot * sizeof(int));
    lockURLs(urlRanks, lockedRanks, nURLsTot, nFiles);
    int nLocks = countNumLocks(lockedRanks, nURLsTot);
    // for (int i = 0; i < nURLsTot; ++i)
    {
        // printf("locked urls: %d\n", lockedRanks[i]);
    }
    // int nRemVals = nURLsTot - nLocks;

    int *vals = malloc(nURLsTot * sizeof(int));
    
    // int cnt = 0;
    for (int i = 0; i < nURLsTot; ++i)
    {
        if(lockedRanks[i] != -1){
            vals[i] = lockedRanks[i];
            if(lockedRanks[i] != i+1)  vals[lockedRanks[i]] = i+1;

            // printf("%d is locked up as %d\n", i, vals[i]);
        } else if(vals[i] == 0){
            vals[i] = i+1;
        }
    }



    int maxNPVals = fact(nURLsTot);
    int **pVals = malloc(maxNPVals* sizeof(int *));
    char **pStrings = malloc(maxNPVals* sizeof(char *));

    for (int j = 0; j < maxNPVals; ++j)
    {
        pVals[j] = malloc(nURLsTot * sizeof(int));
        pStrings[j] = malloc(2*nURLsTot+1);
    }

    Set perms = newSet();


    permutation(vals, 0, nURLsTot, lockedRanks, perms);

    // showSet(perms);

    pStrings = getElemArr(perms);
    // showSet(perms);
    int numPVals = fact(nURLsTot-nLocks);

    char *lineInPStrings = malloc(2* nURLsTot +1);
    char **rankArrFromPStrings = malloc(sizeof(char *));


    for (int i = 0; i < numPVals; ++i)
    {
        strcpy(lineInPStrings, pStrings[i]);
        rankArrFromPStrings[0] = malloc(nURLsTot+1);
        rankArrFromPStrings = parseStringBySpaces(lineInPStrings);


        for (int j = 0; j < nURLsTot; ++j)
        {
            // printf("j: %d, i: %d\n", j, i);
            // printf("%c, ", pStrings[i][j]);
            pVals[i][j] = (int)(rankArrFromPStrings[j][0]) - 48;
            // printf("%d ", pVals[j][i]);
        }    
        // printf("\n");
    }

    float *sfrVals = malloc(numPVals * sizeof(float));
    fillSFRVals(sfrVals, pVals, urlRanks, nURLsTot, numPVals, nFiles, nURLsInFile, fileNames, allURLArr);

    int minSFRPos = findMinPos(sfrVals, numPVals);
    printResults(minSFRPos, sfrVals, pVals, allURLArr, nURLsTot);


    // int *factAdjust = {0, 0, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 362880, 39916800, 479001600}; //array to be used for permutations
    // int fAdjSize = 14;
    
    return 0;
}

int findMinPos(float *sfrVals, int numPVals)
{
    int minPos = 0;
    for (int i = 0; i < numPVals; ++i)
    {
        if(sfrVals[i] < sfrVals[minPos]) minPos = i; 
    }
    return minPos;
}

void printResults(int minPos, float *sfrVals, int **pVals, char **allURLArr, int nURLsTot)
{
    printf("%f\n", sfrVals[minPos]);
    for (int i = 0; i < nURLsTot; ++i)
    {
        printf("%s\n", allURLArr[pVals[minPos][i]-1]);
    }
}

//SFRVals[i] is sum of abs(urlrank[i][j]/nURLsInFile[i] - pVals[i][j]/nURLsTot)
void fillSFRVals(float *sfrVals, int **pVals, int **urlRanks, int nURLsTot, int numPVals,int nFiles, int *nURLsInFile, char **fileNames, char **allURLs)
{
    float urlRank = -1.0;
    float pValRank = -1.0;
    float nURLsF = 0.0;
    float sum = 0.0;
    float part = 0.0;
    for (int i = 0; i < numPVals; ++i)
    {
        sum = 0.0;
       
            for (int k = 0; k < nFiles; ++k)
            {
                // printf("File: %s\n", fileNames[k]);
                for (int j = 0; j < nURLsTot; ++j){
                    urlRank = (float)urlRanks[k][j];
                    pValRank = (float)pVals[i][j];
                    nURLsF = (float)nURLsInFile[k];

                    float p1 = urlRank/nURLsF;
                    float p2 = (pValRank/(float)nURLsTot);
                    part = fabs(p1-p2);

                    sum += part;
                }
            }
            
    
        sfrVals[i] = sum;
        // printf("val in sfrVals %d is %f\n", i, sum);
    }
    
}

int countNumLocks(int *lockedRanks, int nURLsTot){
    int cnt = 0;
    for (int i = 0; i < nURLsTot; ++i)
    {
        if(lockedRanks[i] != -1) cnt++;
    }
    return cnt;
}


void swap(int *fir, int *sec)
{
    int temp = *fir;
    *fir = *sec;
    *sec = temp;
    // printf("swapped %d and %d\n", *fir, *sec);
}
void printVals(int *vals, int n)
{
    printf("Printing Vals: ");
    for (int i = 0; i < n; ++i)
    {
        printf("%d, ", vals[i]);
    }
    printf("\n");
}
/* vals is the string, curr is the current index to start permutation from and size is sizeof the vals */
void permutation(int * vals, int curr, int size, int *locks, Set seen)
{
    // printVals(locks, size);

    if(curr == size-1)
    {
        char *temp = getPermString(vals, size);
        // printf("temp is: %s\n", temp);
        insertInto(seen, temp);


        // if(pVals[cnt][0] == 0){

            // for(int a=0; a<size; a++){
            //     printf("%d", vals[a]);
            //     pVals[cnt][a] = vals[a];            
            // };

            // printf("\n");      
        // }
    }

    else if(locks[curr] != -1){
                // printf("ELSE IF\n");


        permutation(vals, curr+1, size, locks, seen);

    }   else  {
        // printf("ELSE\n");
        for(int i=curr; i<size; i++)
        {
            swap(&vals[curr], &vals[i]);
            permutation(vals, curr+1, size, locks, seen);
            swap(&vals[curr], &vals[i]);
        }
    }
}


int checkArgs(int n) 
{
    return (n <= 1 ? 0 : 1); // checks that the amount of files given is greater than or equal to 1, can't just have 1 rank file lmao
}

void lockURLs(int **urlRanks, int *lockedRanks, int nURLsTot, int nFiles)
{
    int cap = nFiles/2 +1;
    // printf("%d, %d\n", nFiles, cap);
    int cnt = 0;

    for (int i = 0; i < nURLsTot; ++i)
    {
        lockedRanks[i] = -1;
        for (int j = 0; j < nFiles; ++j)
        {
            for (int k = j; k < nFiles; ++k)
            {
                if (urlRanks[j][i] != -1 && urlRanks[j][i] == urlRanks[k][i])
                {
                    cnt++;
                    if(cnt > cap) {
                        lockedRanks[urlRanks[j][i] - 1] = i+1;
                        // printf("a rank locked as %d\n", i);
                        break;
                    } //else printf("fuck\n");

                }
            }
        }
        cnt = 0;
        // printf("its running\n");

    }
}

int fact(int n)
{
    if (n == 1)
        return 1;
    else
        return n * fact(n - 1);
}


//find the index in strArr that string is found in
int findStringPos(char * string, char **strArr, int numStrings)
{
    int pos;

    for (pos = 0; pos < numStrings; pos++)
    {
        if (strcmp(string, strArr[pos]) == 0)
        {
            return pos;
        }
    }

    assert(pos < numStrings);
    return 0;
}

void fillNURLsInFile(int *nURLArr, char **fileNames, int nFiles)
{
    char urls[BUFSIZE];
    for (int i = 0; i < nFiles; ++i)
    {
        // strcpy(fName, fileNames[i]);
        int cnt = 0;
        strcpy(urls, getURLsInFile(fileNames[i]));
        for (int j = 0; j < strlen(urls)-2; ++j)
        {
            if(urls[j] == 'u' && urls[j+1] == 'r' && urls[j+2] == 'l') cnt++;
        }
        nURLArr[i] = cnt;
    }

}


void fillAllURLSet(Set s, char**fileNames, int nFiles)
{
    char urls[BUFSIZE];
    int nURLs = 0;
    char **urlArr;
    for (int i = 0; i < nFiles; ++i)
    {
        // strcpy(fName, fileNames[i]);
        strcpy(urls, getURLsInFile(fileNames[i]));
        if (strcmp(urls, "") == 0)
        {
            return;
        }

        nURLs = numWords(urls);
        urlArr = malloc(nURLs * sizeof(char *));
        urlArr = parseStringBySpaces(urls);
        for (int j = 0; j < nURLs; ++j)
        {
            if(strstr(urlArr[j], "url") != NULL){
                insertInto(s, urlArr[j]);
                // printf("inserted: %s\n", urlArr[j]);
            }
        }
    }
}

char *getURLsInFile(char *fName)
{
    FILE *f;
    if(strstr(fName, ".txt") == NULL){
        f = fopen(makeURLFile(fName), "r");  //open file
    } else {
        f = fopen(fName, "r");  //open file
    }

    if(f == NULL) return "";

    char line[BUFSIZE];                         //var to hold line of text for loop below
    
    int len = 0, prevLen = 0, cnt = 0, addPos = 0;          //counter variables
    int end = 0;                                    //end of last url added, to properly finish full string


    char *allURLs = malloc(150);                //to hold all URLs in one String
    char lastChar = 'a';                        //to track last character added to 'allURLs'

    while(fgets(line, BUFSIZE, f) != NULL) { //read File line by line, store in String 'line'

        len = strlen(line);
        cnt = 0;
        
        while(cnt < len){

            if(line[cnt] == '\n'){          //about to add newline or
                    allURLs[addPos++] = ' ';    //add space to allURLs
                    lastChar = allURLs[addPos-1];   //update lastChar added
                    end = prevLen + cnt;
                    cnt++;  
                    continue;   
                }

                if(lastChar == ' ' && line[cnt] == ' '){ //if would cause more than one " " in allURLs
                    cnt++;
                    continue;
                }

            allURLs[addPos++] = line[cnt];      //add new char to allURLs
            lastChar = line[cnt];               //update lastChar added
            end = prevLen + cnt;
            cnt++;
        }

        prevLen += len;     //to keep allURLs from writing over itself when adding a new line

    }



    if(allURLs[end] != ' ') end++;      //increment to ensure not writing over url address


    allURLs[end] = '\0';                //finish string

    return allURLs;

}


void fillURLRanks(int **urlRanks, int nFiles, int nURLs, char **urlArr, char **fArr)
{
    char url[9];
    char file[BUFSIZE];

    for (int i = 0; i < nURLs; ++i)
    {
        strcpy(url, urlArr[i]);
        // printf("%.5s\n", url);
        for (int j = 0; j < nFiles; ++j)
        {
            strcpy(file, fArr[j]);
            urlRanks[j][i] = getRank(file, url); 
            // printf("%5.s %5.d", file, urlRanks[i][j]);
        }
        // printf("\n\n");
    }
}

int getRank(char fileName[], char *url) {
    FILE *f = fopen(fileName, "r"); // Opens the file to read the ranks inside
    if (f == NULL) exit(EXIT_FAILURE);
    
    char line[BUFSIZE];
    int cnt = 1;


    while(fgets(line, BUFSIZE, f) != NULL) { //read File line by line, store in String 'line'
        if(strstr(line, url) != 0) return cnt;
        cnt++;
    }
    return -1;
}

char *getPermString(int *arr, int size)
{
    char *str = malloc(size*2+1);    
    for (int i = 0; i < size; ++i)
    {
        str[2*i] = arr[i] + '0';
        str[2*i+1] = ' ';
    }
    str[2*size+1] = '\0';
    return str;
}




//prints any array of strings
//for testing purposes

void printStringArr(char ** arr, int n) 
{

    printf("\n\nTesting: %d\n", n);

    for (int i = 0; i < n; i++)
    {
        printf("%s\n", arr[i]);
    }
}
