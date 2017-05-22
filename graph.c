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
   int urlCnt;
} GraphRep;

Graph newGraph(int nV)
{
   assert(nV >= 0);
   int i, j;
   int **e = malloc(nV * sizeof(int *));
   assert(e != NULL);
   for (i = 0; i < nV; i++) {
      e[i] = malloc(nV * sizeof(int));
      assert(e[i] != NULL);
      for (j = 0; j < nV; j++)
         e[i][j] = 0;
   }
   Graph g = malloc(sizeof(GraphRep));
   assert(g != NULL);
   g->nV = nV;  g->nE = 0;  g->edges = e; g->urls = calloc(nV, sizeof(char *)); g->urlCnt = 0;
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
   // printf("Added: %s\n", newURL);
   g->urls[g->urlCnt++] = newURL;
   // printf("Array: %s\n", g->urls[g->urlCnt - 1]);

}

void printURLs(Graph g){
   // printf("begginning to print\n");
   // for (int i = 0; i < g->urlCnt; i++)
   // {
   //    int len = strlen(g->urls[i]);
   //    printf("%d\n", len);

   //    for (int k = 0; k < 5; k++)
   //    {
   //       printf("%c", g->urls[i][k]);
   //    }
   //    printf("%d\n", len);

   // }
}
