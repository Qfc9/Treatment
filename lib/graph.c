#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include <arpa/inet.h>

#include "payloads.h"
#include "graph.h"

// Initializing Functions
static void _graphDestoryNodes(struct _node *n);
static void _graphDestoryEdges(struct _edge *e);
static struct _node *_graphFindNode(struct _node *n, uint32_t value);
static void _graphResetNodes(struct _node *n);
static void _freeStack(struct _stack *s);
static void _graphResetNodes(struct _node *n);
static struct _node *_graphFind(struct _node *n, uint32_t value);

// Creating Graph
graph graphCreate(void)
{
    graph g = calloc(1, sizeof(*g));
    g->nodes = NULL;
    g->payload = NULL;
    g->type = BST;

    return g;
}

void graph_size(struct _node *n, uint32_t *size)
{
    if (!n)
    {
        return;
    }

    (*size)++;
    graph_size(n->next, size);

    return;
}

void graph_set_payload(struct molecule *m, struct _node *n, uint32_t *idx)
{
    if (!n)
    {
        return;
    }

    m[(*idx)].data = htonl(n->data.value);
    if (!n->edges->node)
    {
        m[(*idx)].left = 0;
    }
    else
    {
        m[(*idx)].left = htons(n->edges->node->data.value);
    }

    if (!n->edges->next->node)
    {
        m[(*idx)].right = 0;
    }
    else
    {
        m[(*idx)].right = htons(n->edges->next->node->data.value);
    }

    (*idx)++;
    graph_set_payload(m, n->next, idx);
}

uint32_t graph_payload(graph g)
{
    uint32_t size = 0;
    uint32_t idx = 0;
    graph_size(g->nodes, &size);

    g->payload = calloc(8, size);

    graph_set_payload(g->payload, g->nodes, &idx);

    return size * 8;
}

void _graphPrintEdges(struct _edge *e)
{
    if (!e)
    {
        return;
    }

    if (!e->node)
    {
        printf("E: 0 ");
    }
    else
    {
        printf("E: %u ", e->node->data.value);
    }

    _graphPrintEdges(e->next);
}

void _graphPrintNodes(struct _node *n)
{
    if (!n)
    {
        return;
    }

    printf("D: %u ", n->data.value);
    _graphPrintEdges(n->edges);
    printf("\n");

    _graphPrintNodes(n->next);
}

// Printing the graph
void graphPrint(graph g)
{
    if(!g || !g->nodes)
    {
        return;
    }

    _graphPrintNodes(g->nodes);
}

unsigned int graph_evaluate(struct _node *n)
{
    if (!n)
    {
        return BST;
    }
    
    if (n->edge_sz >= 2)
    {
        return GRAPH;
    }

    return graph_evaluate(n->next);
}

void graph_edge_count_deduction(struct _node *n)
{
    if (!n)
    {
        return;
    }

    if (n->edges->node)
    {
        n->edges->node->edge_sz--;
    }
    if (n->edges->next->node)
    {
        n->edges->next->node->edge_sz--;
    }

    graph_edge_count_deduction(n->next);
}

void graph_add_existing_node(struct _node *n, struct _node *new_n)
{
    if (!n)
    {
        return;
    }
    if (!n->next)
    {
        n->next = new_n;
        return;
    }

    graph_add_existing_node(n->next, new_n);
}

// Adding a node to the graph
void graphAddNode(graph g, uint32_t value)
{
    if(!g)
    {
        return;
    }

    if(!g->nodes)
    {
        g->nodes = calloc(1, sizeof(_node));
        if(!g->nodes)
        {
            return;
        }
        g->nodes->data.value = value;
        g->nodes->visited = false;
        g->nodes->edge_sz = 0;
        g->nodes->parent = NULL;
        return;
    }

    struct _node *newNode = calloc(1, sizeof(_node));
    newNode->data.value = value;
    newNode->visited = false;
    newNode->edge_sz = 0;
    newNode->parent = NULL;

    struct _node *next = g->nodes;
    while(next->next)
    {
        next = next->next;
    }

    next->next = newNode;
}

// Adding an edge to the graph
void graphAddEdge(graph g, uint32_t n1, uint32_t n2)
{
    if(!g || !g->nodes)
    {
        return;
    }

    struct _node *a = _graphFind(g->nodes, n1);
    struct _node *b = _graphFind(g->nodes, n2);

    if (!a)
    {
        return;
    }

    if (b)
    {
        b->edge_sz++;
        if (b->edge_sz >= 2)
        {
            g->type = GRAPH;
        }
    }

    struct _edge *newEdge = calloc(1, sizeof(*newEdge));
    newEdge->node = b;

    struct _edge *curEdge = a->edges;

    if(!a->edges)
    {
        a->edges = newEdge;
        return;
    }

    while(curEdge->next)
    {
        curEdge = curEdge->next;
    }

    curEdge->next = newEdge;

}

// Destroying the graph
void graphDestroy(graph g)
{
    _graphDestoryNodes(g->nodes);

    free(g);
}

// Find a certain node
static struct _node *_graphFind(struct _node *n, uint32_t value)
{
    if(!n)
    {
        return NULL;
    }

    if(n->data.value == value)
    {
        return n;
    }

    return _graphFind(n->next, value);
}

// Freeing a Stack
static void _freeStack(struct _stack *s)
{
    if(!s)
    {
        return;
    }
    _freeStack(s->next);
    free(s);
}

// Settings all nodes to false
void graphResetNodes(graph g)
{
    if(!g)
    {
        return;
    }

    _graphResetNodes(g->nodes);
}

// Settings all nodes to false
static void _graphResetNodes(struct _node *n)
{
    if(!n)
    {
        return;
    }

    n->visited = false;
    n->parent = NULL;
    _graphResetNodes(n->next);
}

// Find a certain node
static struct _node *_graphFindNode(struct _node *n, uint32_t value)
{
    if(!n)
    {
        return NULL;
    }

    if(n->data.value == value)
    {
        return n;
    }

    return _graphFindNode(n->next, value);
}

// Destroy edges
static void _graphDestoryEdges(struct _edge *e)
{
    if(!e)
    {
        return;
    }

    _graphDestoryEdges(e->next);
    free(e);
}

// Destroy nodes and edges
static void _graphDestoryNodes(struct _node *n)
{
    if(!n)
    {
        return;
    }

    _graphDestoryEdges(n->edges);
    _graphDestoryNodes(n->next);
    free(n);
}