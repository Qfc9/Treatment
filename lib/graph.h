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
    unsigned int sz;
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
    size_t edge_inbound;
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
    bool out_of_bounds;
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
void graph_add_edge(graph g, uint32_t n1, uint32_t n2);
bool graphPrintPath(graph g, char **data, size_t sz, char start, char end, size_t search);
void graphDestroy(graph g);

void graph_mark(struct _node *n_list, struct _node *n);

struct _node *_graphFind(struct _node *n, uint32_t value);
void graph_replace_edges(struct _node *rem_n, struct _node *cur_n);
unsigned int graph_evaluate(struct _node *n);
void graph_edge_count_deduction(struct _node *n);
void graph_add_existing_node(struct _node *n, struct _node *new_n);
uint32_t graph_payload(graph g);
uint32_t graph_payload2(graph g);
void graph_size(struct _node *n, uint32_t *size);
void graph_size_ignore(struct _node *n, uint32_t *size);

#endif
