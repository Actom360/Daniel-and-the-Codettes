// main file


/* basic directed graph type */
typedef struct GraphRep *Graph;

// is a vertex valid in a given Graph?
static int validV(Graph g, Vertex v)
{
   return (g != NULL && v >= 0 && v < g->nV);
}

typedef struct GraphRep {
   int   nV;    // #vertices
   int   nE;    // #edges
   int **edges; // matrix of booleans
   char **urls; // array for urls
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
   g->nV = nV;  g->nE = 0;  g->edges = e; g->urls = calloc(nV, sizeof(char *));
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


