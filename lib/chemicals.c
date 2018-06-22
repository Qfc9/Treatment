#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>

#include "graph.h"
#include "payloads.h"
#include "chemicals.h"
#include "util.h"

const char *salt = "I Hate Liam Echlin";

void free_chem_idx(struct chemical_idx *chem);
void add_chemical(struct chemical_idx *chems, struct chemical_idx *new_chem);

struct chemicals* analyze(struct molecule *m_buff, uint16_t sz)
{
    struct chemicals *chems = calloc(1, sizeof(*chems));
    if (!chems)
    {
        printf("ERROR: chems calloc\n");
    }
    chems->total_sz = sz;
    chems->sz = sz;
    chems->chemicals = m_buff;
    chems->hazmat_g = graphCreate();
    if (!chems->hazmat_g)
    {
        printf("ERROR: hazmat_g calloc\n");
    }
    chems->chemicals_g = graphCreate();
    if (!chems->chemicals_g)
    {
        printf("ERROR: chemicals_g calloc\n");
    }
    chems->sludge_g = graphCreate();
    if (!chems->sludge_g)
    {
        printf("ERROR: sludge_g calloc\n");
    }
    chems->trash_g = graphCreate();
    if (!chems->trash_g)
    {
        printf("ERROR: trash_g calloc\n");
    }

    chems->chlorine_max = (unsigned int)((sz / 8) * 0.05);
    chems->chlorine_min = (unsigned int)((sz / 8) * 0.03);


    struct molecule m;
    struct molecule mLink;

    for (uint16_t i = 0; i < (sz / 8); ++i)
    {   
        m = m_buff[i];
        m.data = ntohl(m.data);
        m.left = ntohs(m.left);
        m.right = ntohs(m.right);

        if (m.data == 0)
        {
            if (m.left != 0)
            {
                for (uint16_t n = 0; n < (sz / 8); ++n)
                {
                    if ((ntohs(m_buff[n].left) - 1) == i)
                    {
                        m_buff[n].left = m_buff[i].left;
                    }
                }
            }
            if (m.right != 0)
            {
                for (uint16_t n = 0; n < (sz / 8); ++n)
                {
                    if ((ntohs(m_buff[n].right) - 1) == i)
                    {
                        m_buff[n].right = m_buff[i].right;
                    }
                }
            }
        }
    }

    for (uint16_t i = 0; i < (sz / 8); ++i)
    {   
        m = m_buff[i];
        m.data = ntohl(m.data);
        m.left = ntohs(m.left);
        m.right = ntohs(m.right);

        if (m.data != 0)
        {
            graphAddNode(chems->chemicals_g, m.data);
        }
    }

    for (uint16_t i = 0; i < (sz / 8); ++i)
    {   
        m = m_buff[i];
        m.data = ntohl(m.data);
        m.left = ntohs(m.left);
        m.right = ntohs(m.right);

        if (m.data == 0)
        {
            // graph_replace_edges(mov_n, chems->chemicals_g->nodes);
            continue;
        }

        if (m.left != 0)
        {
            mLink = m_buff[(m.left - 1)];
            mLink.data = ntohl(mLink.data);;

            graphAddEdge(chems->chemicals_g, m.data, mLink.data);
        }
        else
        {
            graphAddEdge(chems->chemicals_g, m.data, 0);
        }

        if (m.right != 0)
        {
            mLink = m_buff[(m.right - 1)];
            mLink.data = ntohl(mLink.data);

            graphAddEdge(chems->chemicals_g, m.data, mLink.data);
        }
        else
        {
            graphAddEdge(chems->chemicals_g, m.data, 0);
        }

    }

    graph_size(chems->chemicals_g->nodes, &chems->sz);

    return chems;
}

void add_chemical(struct chemical_idx *chems, struct chemical_idx *new_chem)
{
    if (!chems)
    {
        return;
    }

    if (!chems->next)
    {
        chems->next = new_chem;
        return;
    }

    add_chemical(chems->next, new_chem);

    return;
}

int trash_detect(struct chemicals *chems)
{
    struct _node *cur_n = chems->chemicals_g->nodes;

    while(cur_n)
    {
        if ((cur_n->edges->next->node && !_graphFind(chems->chemicals_g->nodes, cur_n->edges->next->node->data.value))
        || (cur_n->edges->node && !_graphFind(chems->chemicals_g->nodes, cur_n->edges->node->data.value))
        || cur_n->edges->out_of_bounds || cur_n->edges->next->out_of_bounds)
        {
            return 1;
        }

        cur_n = cur_n->next;
    }

    return 0;
}


void remove_trash(struct chemicals *chems)
{
    struct _node *mov_n = NULL;
    struct _node *cur_n = chems->chemicals_g->nodes;
    struct _node *prev_n = NULL;

    while(cur_n)
    {

        if ((cur_n->edges->next->node && !_graphFind(chems->chemicals_g->nodes, cur_n->edges->next->node->data.value))
        || (cur_n->edges->node && !_graphFind(chems->chemicals_g->nodes, cur_n->edges->node->data.value))
        || cur_n->edges->out_of_bounds || cur_n->edges->next->out_of_bounds)
        {
            mov_n = cur_n;
            cur_n = cur_n->next;
            mov_n->next = NULL;
            
            if (!prev_n)
            {
                chems->chemicals_g->nodes = cur_n;
            }
            else
            {
                prev_n->next = cur_n;
            }
            
            if (!chems->trash_g->nodes)
            {
                chems->trash_g->nodes = mov_n;
            }
            else
            {
                graph_add_existing_node(chems->trash_g->nodes, mov_n);
            }

            graph_edge_count_deduction(mov_n);
        }
        else
        {
            prev_n = cur_n;
            cur_n = cur_n->next;
        }
    }
    chems->chemicals_g->type = graph_evaluate(chems->chemicals_g->nodes);
}

void remove_ammonia(struct chemicals *chems)
{
    struct _node *mov_n = NULL;
    struct _node *cur_n = chems->chemicals_g->nodes;
    struct _node *prev_n = NULL;

    while(cur_n)
    {
        if (is_undulating(cur_n->data.value))
        {
            mov_n = cur_n;
            cur_n = cur_n->next;
            mov_n->next = NULL;
            
            if (!prev_n)
            {
                chems->chemicals_g->nodes = cur_n;
            }
            else
            {
                prev_n->next = cur_n;
            }
            
            if (!chems->sludge_g->nodes)
            {
                chems->sludge_g->nodes = mov_n;
            }
            else
            {
                graph_add_existing_node(chems->sludge_g->nodes, mov_n);
            }

            graph_replace_edges(mov_n, chems->chemicals_g->nodes);
            graph_edge_count_deduction(mov_n);
        }
        else
        {
            prev_n = cur_n;
            cur_n = cur_n->next;
        }
    }
    chems->chemicals_g->type = graph_evaluate(chems->chemicals_g->nodes);
}

void remove_feces(struct chemicals *chems)
{
    struct _node *mov_n = NULL;
    struct _node *cur_n = chems->chemicals_g->nodes;
    struct _node *prev_n = NULL;

    while(cur_n)
    {
        if (is_prime(cur_n->data.value))
        {
            mov_n = cur_n;
            cur_n = cur_n->next;
            mov_n->next = NULL;
            
            if (!prev_n)
            {
                chems->chemicals_g->nodes = cur_n;
            }
            else
            {
                prev_n->next = cur_n;
            }
            
            if (!chems->sludge_g->nodes)
            {
                chems->sludge_g->nodes = mov_n;
            }
            else
            {
                graph_add_existing_node(chems->sludge_g->nodes, mov_n);
            }

            graph_replace_edges(mov_n, chems->chemicals_g->nodes);
            graph_edge_count_deduction(mov_n);
        }
        else
        {
            prev_n = cur_n;
            cur_n = cur_n->next;
        }
    }
    chems->chemicals_g->type = graph_evaluate(chems->chemicals_g->nodes);
}

void remove_lead(struct chemicals *chems)
{
    struct _node *mov_n = NULL;
    struct _node *cur_n = chems->chemicals_g->nodes;
    struct _node *prev_n = NULL;

    while(cur_n)
    {
        if (cur_n->edge_sz == 0)
        {
            mov_n = cur_n;
            cur_n = cur_n->next;
            mov_n->next = NULL;
            
            if (!prev_n)
            {
                chems->chemicals_g->nodes = cur_n;
            }
            else
            {
                prev_n->next = cur_n;
            }
            
            if (!chems->hazmat_g->nodes)
            {
                chems->hazmat_g->nodes = mov_n;
            }
            else
            {
                graph_add_existing_node(chems->hazmat_g->nodes, mov_n);
            }
            graph_edge_count_deduction(mov_n);
        }
        else
        {
            prev_n = cur_n;
            cur_n = cur_n->next;
        }
    }
    chems->chemicals_g->type = graph_evaluate(chems->chemicals_g->nodes);
}

int lead_detect(struct _node *n)
{
    if (!n)
    {
        return 0;
    }

    if (n->edge_sz == 0)
    {
        return 1;
    }

    return lead_detect(n->next);
}

void free_chemicals(struct chemicals *chems)
{
    if (!chems)
    {
        return;
    }
    free_chem_idx(chems->chlorine);
    free(chems->chemicals);
    graphDestroy(chems->chemicals_g);
    graphDestroy(chems->hazmat_g);
    free(chems);
}

void free_chem_idx(struct chemical_idx *chem)
{
    if (!chem)
    {
        return;
    }
    free_chem_idx(chem->next);
    free(chem);
}
