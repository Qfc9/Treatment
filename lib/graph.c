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
static void _graphResetNodes(struct _node *n);
static void _graphResetNodes(struct _node *n);

// Creating Graph
graph graphCreate(void)
{
    graph g = calloc(1, sizeof(*g));
    g->nodes = NULL;
    g->payload = NULL;
    g->type = BST;

    return g;
}

void graph_mark_edges(struct _node *n)
{
    if(!n)
    {
        return;
    }

    if (n->edges->node && !n->edges->node->visited)
    {
        n->edges->node->visited = true;
        graph_mark_edges(n->edges->node);
    }
    if (n->edges->next->node && !n->edges->next->node->visited)
    {
        n->edges->next->node->visited = true;
        graph_mark_edges(n->edges->next->node);
    } 

}

void graph_mark(struct _node *n_list, struct _node *n)
{
    if (!n_list)
    {
        return;
    }

    if (n_list != n)
    {
        if (n_list->edges->node)
        {
            if (n_list->edges->node == n || n_list->edges->node->visited)
            {
                n_list->visited = true;
                graph_mark_edges(n_list->edges->node);
            }

        }
        else if (n_list->edges->next->node)
        {
            if (n_list->edges->next->node == n || n_list->edges->next->node->visited)
            {
                n_list->visited = true;
                graph_mark_edges(n_list->edges->next->node);
            }
        } 
    }
    else
    {
        graph_mark_edges(n_list);
    }

    graph_mark(n_list->next, n);

}

void graph_replace_edges(struct _node *rem_n, struct _node *cur_n)
{
    if (!cur_n)
    {
        return;
    }

    if (cur_n->edges->node)
    {
        if (cur_n->edges->node == rem_n)
        {
            cur_n->edges->node = rem_n->edges->node;
            if (rem_n->edges->node)
            {
                cur_n->edges->node->edge_inbound++;
            }
        }
    }

    if (cur_n->edges->next->node)
    {
        if (cur_n->edges->next->node == rem_n)
        {
            cur_n->edges->next->node = rem_n->edges->next->node;
            if (cur_n->edges->next->node)
            {
                cur_n->edges->next->node->edge_inbound++;
            }
        }
    }

    graph_replace_edges(rem_n, cur_n->next);
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

void graph_size_ignore(struct _node *n, uint32_t *size)
{
    if (!n)
    {
        return;
    }

    if (n->data.value != 0)
    {
        (*size)++;
    }
    graph_size(n->next, size);

    return;
}

uint16_t graph_node_idx(struct _node *n, uint32_t data)
{
    if (!n)
    {
        return 0;
    }

    uint16_t idx = 1;

    while(n)
    {
        if (n->data.value == data)
        {
            return idx;
        }
        idx++;
        n = n->next;
    }

    return 0;
}

void graph_set_payload(graph g, struct _node *n, uint32_t *idx)
{
    if (!n)
    {
        return;
    }

     g->payload[(*idx)].data = htonl(n->data.value);
    if (!n->edges->node)
    {
        g->payload[(*idx)].left = 0;
    }
    else
    {
        g->payload[(*idx)].left = htons(graph_node_idx(g->nodes, n->edges->node->data.value));
    }

    if (!n->edges->next->node)
    {
        g->payload[(*idx)].right = 0;
    }
    else
    {
        g->payload[(*idx)].right = htons(graph_node_idx(g->nodes, n->edges->next->node->data.value));
    }

    if (n->edges->out_of_bounds || n->edges->next->out_of_bounds)
    {
        g->payload[(*idx)].right = 0xFFFF;
        g->payload[(*idx)].left = 0xFFFF;
    }

    (*idx)++;
    graph_set_payload(g, n->next, idx);
}

uint32_t graph_payload(graph g)
{
    if (!g->nodes)
    {
        return 0;
    }

    uint32_t size = 0;
    uint32_t idx = 0;
    graph_size(g->nodes, &size);

    g->payload = calloc(8, size);

    graph_set_payload(g, g->nodes, &idx);

    g->sz = size * 8;

    return size * 8;
}

void _graph_print_nodes(struct _node *n)
{
    if (!n)
    {
        return;
    }

    printf("D: %u ", n->data.value);
    if (n->edges->node)
    {
        printf("E: %u ", n->edges->node->data.value);
    }
    else
    {
        printf("E: 0 ");
    }
    if (n->edges->next->node)
    {
        printf("E: %u ", n->edges->next->node->data.value);
    }
    else
    {
        printf("E: 0 ");
    }
    printf("\n");

    _graph_print_nodes(n->next);
}

// Printing the graph
void graphPrint(graph g)
{
    if(!g || !g->nodes)
    {
        return;
    }

    _graph_print_nodes(g->nodes);
}

unsigned int graph_evaluate(struct _node *n)
{
    if (!n)
    {
        return BST;
    }
    
    if (n->edge_inbound >= 2)
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

    if (n->edges->node != NULL && n->edges->node == n->edges->next->node)
    {
        n->edges->node->edge_inbound--;
        return;
    }

    if (n->edges->node)
    {
        n->edges->node->edge_inbound--;
    }
    if (n->edges->next->node)
    {
        n->edges->next->node->edge_inbound--;
    }
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
        g->nodes->edge_inbound = 0;
        g->nodes->parent = NULL;
        return;
    }

    struct _node *newNode = calloc(1, sizeof(_node));
    newNode->data.value = value;
    newNode->visited = false;
    newNode->edge_inbound = 0;
    newNode->parent = NULL;

    struct _node *next = g->nodes;
    while(next->next)
    {
        next = next->next;
    }

    next->next = newNode;
}

struct _node *graph_find_by_idx(graph g, uint32_t idx)
{
    struct _node *n = g->nodes;

    if (!n || idx == 0)
    {
        return NULL;
    }

    uint32_t current_pos = 1;
    while(n)
    {
        if(current_pos == idx)
        {
            return n;
        }
        current_pos++;
        n = n->next;
    }

    return NULL;
}

// Adding an edge to the graph
void graph_add_edge(graph g, uint32_t n1_pos, uint32_t n2_pos)
{
    if(!g || !g->nodes)
    {
        return;
    }

    struct _node *a = graph_find_by_idx(g, n1_pos);
    struct _node *b = graph_find_by_idx(g, n2_pos);

    struct _edge *newEdge = calloc(1, sizeof(*newEdge));
    newEdge->node = b;
    newEdge->out_of_bounds = false;

    if (!b && n2_pos != 0)
    {
        newEdge->out_of_bounds = true;
    }

    struct _edge *curEdge = a->edges;

    if(!a->edges)
    {
        if (b)
        {
            b->edge_inbound++;
        }
        a->edges = newEdge;
        return;
    }

    if (b)
    {
        if (!(curEdge->node != NULL && curEdge->node == newEdge->node))
        {
            b->edge_inbound++;
        }
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
struct _node *_graph_find(struct _node *n, struct _node *find_n)
{
    if(!n)
    {
        return NULL;
    }

    if(n == find_n)
    {
        return n;
    }

    return _graph_find(n->next, find_n);
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
