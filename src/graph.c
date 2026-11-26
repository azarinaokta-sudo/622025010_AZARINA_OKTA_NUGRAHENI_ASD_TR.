#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initGraph(Graph *g) {
    g->count = 0;
    for (int i = 0; i < MAX_VERTEX; i++) {
        g->vertices[i].head = NULL;
        g->vertices[i].nama[0] = 0;
    }
}

int findVertex(const Graph *g, const char *nama) {
    for (int i = 0; i < g->count; i++) {
        if (strcmp(g->vertices[i].nama, nama) == 0) return i;
    }
    return -1;
}

int addVertex(Graph *g, const char *nama) {
    int exist = findVertex(g, nama);
    if (exist >= 0) return exist;
    if (g->count >= MAX_VERTEX) return -1;

    snprintf(g->vertices[g->count].nama, MAX_STR, "%s", nama);
    g->vertices[g->count].head = NULL;
    return g->count++;
}

/* Edge tak berarah; vertex otomatis dibuat bila belum ada */
int addEdge(Graph *g, const char *src, const char *dest) {
    int s = addVertex(g, src);
    int d = addVertex(g, dest);
    if (s < 0 || d < 0) return 0;

    AdjNode *n = (AdjNode *)malloc(sizeof(AdjNode));
    if (!n) return 0;
    AdjNode *m = (AdjNode *)malloc(sizeof(AdjNode));
    if (!m) {
        free(n);
        return 0;
    }

    n->dest = d;
    n->next = g->vertices[s].head;
    g->vertices[s].head = n;

    m->dest = s;
    m->next = g->vertices[d].head;
    g->vertices[d].head = m;
    return 1;
}

void BFS(const Graph *g, int start) {
    if (start < 0 || start >= g->count) return;

    int visited[MAX_VERTEX] = {0};
    int queue[MAX_VERTEX];
    int front = 0, rear = 0;

    visited[start] = 1;
    queue[rear++] = start;

    printf(YEL BOLD "BFS dari %s: " RST, g->vertices[start].nama);
    while (front < rear) {
        int v = queue[front++];
        printf(CYN BOLD "%s" RST " " MAG "»" RST " ", g->vertices[v].nama);
        for (AdjNode *cur = g->vertices[v].head; cur; cur = cur->next) {
            if (!visited[cur->dest]) {
                visited[cur->dest] = 1;
                queue[rear++] = cur->dest;
            }
        }
    }
    printf("\n");
}

static void dfsRec(const Graph *g, int v, int *visited) {
    visited[v] = 1;
    printf(CYN BOLD "%s" RST " " MAG "»" RST " ", g->vertices[v].nama);
    for (AdjNode *cur = g->vertices[v].head; cur; cur = cur->next) {
        if (!visited[cur->dest]) dfsRec(g, cur->dest, visited);
    }
}

void DFS(const Graph *g, int start) {
    if (start < 0 || start >= g->count) return;
    int visited[MAX_VERTEX] = {0};
    printf(YEL BOLD "DFS dari %s: " RST, g->vertices[start].nama);
    dfsRec(g, start, visited);
    printf("\n");
}

void freeGraph(Graph *g) {
    for (int i = 0; i < g->count; i++) {
        AdjNode *cur = g->vertices[i].head;
        while (cur) {
            AdjNode *nx = cur->next;
            free(cur);
            cur = nx;
        }
        g->vertices[i].head = NULL;
    }
    g->count = 0;
}

/* Format file: V|nama atau E|asal|tujuan */
int loadPeta(Graph *g, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        char *tok = strtok(line, "|");
        if (!tok) continue;
        if (strcmp(tok, "V") == 0) {
            tok = strtok(NULL, "|");
            if (tok) addVertex(g, tok);
        } else if (strcmp(tok, "E") == 0) {
            char *src = strtok(NULL, "|");
            char *dst = strtok(NULL, "|");
            if (src && dst) addEdge(g, src, dst);
        }
    }
    fclose(f);
    return 1;
}

int savePeta(const Graph *g, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;

    for (int i = 0; i < g->count; i++) {
        fprintf(f, "V|%s\n", g->vertices[i].nama);
    }
    for (int i = 0; i < g->count; i++) {
        for (AdjNode *cur = g->vertices[i].head; cur; cur = cur->next) {
            if (i < cur->dest) {
                fprintf(f, "E|%s|%s\n", g->vertices[i].nama, g->vertices[cur->dest].nama);
            }
        }
    }
    fclose(f);
    return 1;
}
