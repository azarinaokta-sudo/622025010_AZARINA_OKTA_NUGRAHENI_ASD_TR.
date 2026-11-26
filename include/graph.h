#ifndef GRAPH_H
#define GRAPH_H

#include "config.h"

typedef struct AdjNode {
    int dest;
    struct AdjNode *next;
} AdjNode;

typedef struct {
    char     nama[MAX_STR];
    AdjNode *head;
} Vertex;

typedef struct {
    Vertex vertices[MAX_VERTEX];
    int    count;
} Graph;

void initGraph(Graph *g);
int  findVertex(const Graph *g, const char *nama);
int  addVertex(Graph *g, const char *nama);
int  addEdge(Graph *g, const char *src, const char *dest);
void BFS(const Graph *g, int start);
void DFS(const Graph *g, int start);
void freeGraph(Graph *g);
int  loadPeta(Graph *g, const char *filename);
int  savePeta(const Graph *g, const char *filename);

#endif /* GRAPH_H */
