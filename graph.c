/* basic directed graph type */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct GraphRep *Graph;
typedef int Vertex;
typedef struct { Vertex v; Vertex w; } Edge;

int validG(Graph g);
int validE(Graph g, Edge e);

typedef struct GraphRep {
   int   nV;    // #vertices
   int   nE;    // #edges
   int **edges; // matrix of booleans
   char **urls; // array for urls
   int urlCnt;  // num of URLs in graph
} GraphRep;

Graph newGraph(int nV)
{
   assert(nV >= 0);
   int i;
   int **e = calloc(nV, sizeof(int *));
   char ** u = calloc(nV, sizeof(char *));

   assert(e != NULL && u != NULL);

   for (i = 0; i < nV; i++) {
      e[i] =  calloc(nV , sizeof(int));
      u[i] = calloc(5, sizeof(char));

      // assert(e[i] != NULL);
      // for (j = 0; j < nV; j++)
      //    e[i][j] = 0;
   }
   Graph g = malloc(sizeof(GraphRep));
   assert(g != NULL);
   g->nV = nV;  g->nE = 0;  g->edges = e; g->urls = u;  g->urlCnt = 0;
   return g;
}

void insertE(Graph g, Edge e)
{
   assert(validG(g) && validE(g,e));
   if (g->edges[e.v][e.w]) return;
   g->edges[e.v][e.w] = 1;
   g->edges[e.w][e.v] = 1;
   g->nE++;
}

void removeE(Graph g, Edge e)
{
   assert(validG(g) && validE(g,e));
   if (!g->edges[e.v][e.w]) return;
   g->edges[e.v][e.w] = 0;
   g->edges[e.w][e.v] = 0;
   g->nE--;
}

int validG(Graph g) {
	return (g != NULL);   // other checks?
}
int validV(Graph g, Vertex v) {
	return (validG(g) && 0 <= v && v < g->nV);
}
int validE(Graph g, Edge e) {
	return (validV(g,e.v) && validV(g,e.w));
}

void addURL(Graph g, char *newURL){
   printf("add to prove im not tupid%s\n", newURL);
   g->urls[g->urlCnt] = newURL;
   g->urlCnt++;
}

void printURLs(Graph g){
   printf("beginning to print %d urls\n", g->urlCnt);
   // for (int i = 0; i < g->urlCnt; i++)
   {

      // int len = strlen(g->urls[i]);
      // printf("%d : %d\n", len, i);

      int cnt = 0;

      for (int k = 0; k < 6*g->urlCnt; k++)
      {
         if(k == (6*cnt)){
            printf(" (k = %d) ", k);
            cnt++;
            // printf(", ");
         }  

         char c = g->urls[0][k];
         if (c != '\n')
         {
            printf("%c", c); 
         }
         else if (c == '\0')
         {
            printf("N");
         } 
         else
         {
            printf("S");         
         } 

         
      }
      printf("\n\n");


   }
}

int getURLCount(Graph g){
   return g->urlCnt;
}

void freeGraph(Graph g){

}
