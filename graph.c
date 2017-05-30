// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"



// graph representation (adjacency matrix)
typedef struct GraphRep {
   int    nV;    // #vertices
   int    nE;    // #edges
   int  **edges; // connection matrix
} GraphRep;

// check validity of Vertex
int validV(Graph g, Vertex v)
{
   return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
Edge mkEdge(Graph g, Vertex v, Vertex w)
{
   assert(g != NULL && validV(g,v) && validV(g,w));
   Edge new = {v,w}; // struct assignment
   return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge(Graph g, Vertex v, Vertex w)
{
   assert(g != NULL && validV(g,v) && validV(g,w));
   if (g->edges[v][w] == 0) {
      g->edges[v][w] = 1;
      g->nE++;
   }
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge(Graph g, Vertex v, Vertex w)
{
   assert(g != NULL && validV(g,v) && validV(g,w));
   if (g->edges[v][w] != 0) {
      g->edges[v][w] = 0;
      g->nE--;
   }
}

// create an empty graph
Graph newGraph(int nV)
{
   assert(nV > 0);
   Graph g = malloc(sizeof(Graph));
   assert(g != NULL);

   g->nV = nV;
   g->nE = 0;
   g->edges = (int **) malloc(nV* sizeof(int *));
   for (int i = 0; i < nV; i++)
   {
      g->edges[i] = (int *)malloc(nV * sizeof(int));
   }
   for(int col = 0; col < nV; col++){
      for(int row = 0; row < nV; row++){
         g->edges[col][row] = 0;
      }
   }
   return g;
}

// free memory associated with graph
void dropGraph(Graph g)
{
   assert(g != NULL);

   for (int i = 0; i < g->nV; i++)
   {
      free(g->edges[i]);
   }
   free(g->edges);
   free(g);
   return;
}

// display graph, using names for vertices
void showGraph(Graph g, char **names)
{
   assert(g != NULL);
   printf("#vertices=%d, #edges=%d\n\n",g->nV,g->nE);
   int v, w;
   for (v = 0; v < g->nV; v++) {
      printf("%d %s\n",v,names[v]);
      for (w = 0; w < g->nV; w++) {
         if (g->edges[v][w]) {
            printf("\t%s (%d)\n",names[w],g->edges[v][w]);
         }
      }
      printf("\n");
   }
}


int isEdge(Graph g, Vertex i, Vertex j)
{
   assert(validV(g, i) && validV(g, j));
   return (g->edges[i][j]);
}