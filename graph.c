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
} GraphRep;

Graph newGraph(int nV)
{
   assert(nV >= 0);
   int i;
   int **e = calloc(nV, sizeof(int *));

   assert(e != NULL);

   for (i = 0; i < nV; i++) {
      e[i] =  calloc(nV , sizeof(int));

   }
   Graph g = malloc(sizeof(GraphRep));
   assert(g != NULL);
   g->nV = nV;  g->nE = 0;  g->edges = e;
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

