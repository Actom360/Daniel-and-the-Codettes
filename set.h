// set.h ... interface to simple Set of Strings
// Written by John Shepherd, September 2015


// NOTE that sets are stored in alphabetical order

#ifndef SET_H
#define SET_H

typedef struct SetRep *Set;

// Function signatures

Set newSet();
void disposeSet(Set);
void insertInto(Set,char *);
void dropFrom(Set,char *);
int  isElem(Set,char *);
int  nElems(Set);
void showSet(Set);

//returns array of all strings in Set
char **getElemArr(Set s);

//removes and returns 1st string in Set (1st in alphabetical order)
//works well because Sets are kept alphabetically
char *popString(Set s);		


#endif
