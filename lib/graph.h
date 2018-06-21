#ifndef graph_h
#define graph_h
#define DFS 0
#define FAST 1
#define BOTH 2

#include <stdlib.h>
#include <stdbool.h>

enum graph_type {BST=0, GRAPH=1};

struct _graph
{
    unsigned int type;
    struct _node *nodes;
    struct molecule *payload;
} _graph;

// Initializing Structs
struct _data
{
    size_t x;
    size_t y;
    uint32_t value;
} _data;

struct _node
{
    size_t edge_sz;
    struct _data data;
    bool visited;
    struct _node *parent;
    struct _edge *edges;
    struct _node *next;
} _node;

struct _edge
{
    struct _node *node;
    struct _edge *next;  
} _edge;

struct _stack
{
    struct _node *node;
    struct _stack *next;
} _stack;

typedef struct _graph *graph;

struct _node;

graph graphCreate(void);
void graphAddNode(graph g, uint32_t value);
void graphPrint(graph g);
void graphAddEdge(graph g, uint32_t n1, uint32_t n2);
bool graphPrintPath(graph g, char **data, size_t sz, char start, char end, size_t search);
void graphDestroy(graph g);

uint32_t graph_payload(graph g);
void graph_size(struct _node *n, uint32_t *size);

#endif
