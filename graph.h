// graph representation is hidden
typedef struct GraphRep *Graph;

// vertices denoted by integers 0..N-1
typedef int Vertex;

int   validV(Graph,Vertex); //validity check

// edges are pairs of vertices (end-points)
typedef struct { Vertex v; Vertex w; } Edge;

Edge mkEdge(Graph, Vertex, Vertex);

// operations on graphs
Graph newGraph(int nV);  // #vertices
void  insertE(Graph, Edge);
void  removeE(Graph, Edge);
Graph copy(Graph);
void  destroy(Graph);
void  show(Graph);