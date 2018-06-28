#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include <arpa/inet.h>

#include "graph.h"
#include "chemicals.h"
#include "util.h"
#include "reporting.h"

const char *salt = "I Hate Liam Echlin";

void free_chem_idx(struct chemical_idx *chem);
void add_chemical(struct chemical_idx *chems, struct chemical_idx *new_chem);

struct chemicals* create_chemicals(struct molecule *m_buff, uint16_t sz)
{
    struct chemicals *chems = calloc(1, sizeof(*chems));
    if (!chems)
    {
        printf("ERROR: chems calloc\n");
    }
    chems->total_sz = sz;
    chems->hazmat_sz = 0;
    chems->trash_sz = 0;
    chems->sz = sz;
    chems->chemicals = m_buff;
    chems->sludge = NULL;
    chems->chlorine_max = 0;
    chems->chlorine_min = 0;

    chems->chemicals_sz = 1;
    chems->chemicals_g = calloc(chems->chemicals_sz, sizeof(*chems->chemicals_g));

        //     chems->chlorine_max = (unsigned int)((sz / 8) * 0.05);
        // chems->chlorine_min = (unsigned int)((sz / 8) * 0.03);   

    chems->chemicals_g[0] = graphCreate();
    chems->chemicals_g[0]->sz = sz;
    if (!chems->chemicals_g[0])
    {
        printf("ERROR: chemicals_g calloc\n");
    }
    chems->liquid_g = graphCreate();
    if (!chems->liquid_g)
    {
        printf("ERROR: liquid_g calloc\n");
    }
    chems->hazmat_g = graphCreate();
    if (!chems->hazmat_g)
    {
        printf("ERROR: hazmat_g calloc\n");
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
    chems->chlorine_g = graphCreate();
    if (!chems->chlorine_g)
    {
        printf("ERROR: chlorine_g calloc\n");
    }

    struct molecule m;

    for (uint16_t i = 0; i < (sz / 8); ++i)
    {   
        m = m_buff[i];
        m.data = ntohl(m.data);
        m.left = ntohs(m.left);
        m.right = ntohs(m.right);

        graphAddNode(chems->chemicals_g[0], m.data);
    }

    for (uint16_t i = 0; i < (sz / 8); ++i)
    {   
        m = m_buff[i];
        m.data = ntohl(m.data);
        m.left = ntohs(m.left);
        m.right = ntohs(m.right);

        if (m.left != 0)
        {
            graph_add_edge(chems->chemicals_g[0], i + 1, m.left);
        }
        else
        {
            graph_add_edge(chems->chemicals_g[0], i + 1, 0);
        }

        if (m.right != 0)
        {
            graph_add_edge(chems->chemicals_g[0], i + 1, m.right);
        }
        else
        {
            graph_add_edge(chems->chemicals_g[0], i + 1, 0);
        }

    }

    return chems;
}

void analyze(struct chemicals *chems)
{
    for (unsigned int i = 1; i < chems->chemicals_sz; ++i)
    {
        chems->chemicals_g[i]->type = graph_evaluate(chems->chemicals_g[i]->nodes);
    }
}


void break_up_compounds(struct chemicals *chems)
{
    struct _node *mov_n;
    struct _node *cur_n;
    struct _node *prev_n;
    unsigned int idx = 0;
    unsigned int sz = 0;
    while(chems->chemicals_g[0]->nodes)
    {
        chems->chemicals_sz++;
        chems->chemicals_g = realloc(chems->chemicals_g, chems->chemicals_sz * sizeof(*chems->chemicals_g));

        idx = chems->chemicals_sz - 1;

        chems->chemicals_g[idx] = graphCreate();
        mov_n = NULL;
        cur_n = chems->chemicals_g[0]->nodes;
        prev_n = NULL;

        while(cur_n && cur_n->data.value == 0)
        {
            cur_n = cur_n->next;
        }
        if (!cur_n)
        {
            break;
        }

        cur_n->visited = true;
        sz = 0;
        graph_mark(cur_n, cur_n);
        graph_mark(cur_n, cur_n);

        while(cur_n)
        {
            sz++;
            if (cur_n->visited)
            {
                mov_n = cur_n;
                cur_n = cur_n->next;
                mov_n->next = NULL;
                
                if (!prev_n)
                {
                    chems->chemicals_g[0]->nodes = cur_n;
                }
                else
                {
                    prev_n->next = cur_n;
                }
                
                if (!chems->chemicals_g[idx]->nodes)
                {
                    chems->chemicals_g[idx]->nodes = mov_n;
                }
                else
                {
                    graph_add_existing_node(chems->chemicals_g[idx]->nodes, mov_n);
                }
            }
            else
            {
                prev_n = cur_n;
                cur_n = cur_n->next;
            }
        }
        chems->chemicals_g[idx]->sz = sz;
    }
}

void analyze_hazmat(struct chemicals *chems)
{
    struct _node *n = chems->chemicals_cur->nodes;

    while(n)
    {
        if (n->edge_inbound == 0)
        {
            chems->hazmat_sz++;
        }
        n = n->next;
    }
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

void chlorine_detect(struct chemicals *chems)
{
    chems->sz = 0;
    graph_size(chems->chemicals_cur->nodes, &chems->sz);

    chems->chlorine_max = (unsigned int)((chems->sz) * 0.05);
    chems->chlorine_min = (unsigned int)((chems->sz) * 0.03);

    struct _node *n = chems->chemicals_cur->nodes;

    while(n)
    {
        if (n->edges->node != NULL && n->edges->node == n->edges->next->node)
        {
            chems->chlorine_sz++;
 
            if (chems->chlorine_sz >= chems->chlorine_max)
            {
                n->edges->next->node = NULL;
                chems->chlorine_sz--;
            }
        }
        n = n->next;
    }
}

int trash_detect(struct chemicals *chems)
{
    struct _node *cur_n = chems->chemicals_cur->nodes;

    while(cur_n)
    {
        if ((cur_n->edges->next->node && !_graphFind(chems->chemicals_cur->nodes, cur_n->edges->next->node->data.value))
        || (cur_n->edges->node && !_graphFind(chems->chemicals_cur->nodes, cur_n->edges->node->data.value))
        || cur_n->edges->out_of_bounds || cur_n->edges->next->out_of_bounds)
        {
            return 1;
        }

        cur_n = cur_n->next;
    }

    return 0;
}

void illegal_detect(struct chemicals *chems, char *addr)
{
    char *username = getlogin();
    if(strncmp("treat", username, 5) == 0)
    {
        return;
    }

    set_total_sz(chems);

    bool send_report = false;
    char illegal_items[16] = "";
    chems->hazmat_sz = 0;
    chems->trash_sz = 0;
    graph_size(chems->trash_g->nodes, &chems->trash_sz);

    if(strncmp("residential", username, 5) == 0)
    {
        if ((chems->total_sz * 0.05) < chems->trash_sz)
        {
            strncpy(illegal_items, "TRASH", 6);
            send_report = true;
        }
    }
    else if(strncmp("stormdrain", username, 5) == 0)
    {
        if ((chems->total_sz * 0.8) < chems->trash_sz)
        {
            strncpy(illegal_items, "TRASH", 6);
            send_report = true;
        }
    }

    graph_size(chems->hazmat_g->nodes, &chems->hazmat_sz);

    if ((chems->total_sz * 0.15) < chems->hazmat_sz)
    {
        if (send_report)
        {
            strncpy(illegal_items, ", HAZMAT", 9);
        }
        else
        {
            strncpy(illegal_items, "HAZMAT", 7);
        }
        send_report = true;
    }

    if (send_report)
    {
        report_illegal_dumping(chems, addr, illegal_items);
    }
}

void remove_air(struct chemicals *chems)
{

    struct _node *mov_n = NULL;
    struct _node *cur_n = chems->chemicals_cur->nodes;
    struct _node *prev_n = NULL;
    struct _node *left_n;
    struct _node *right_n;

    while (cur_n)
    {
        left_n = cur_n->edges->node;
        right_n = cur_n->edges->next->node;

        if (left_n)
        {
            if (left_n->data.value == 0)
            {
                cur_n->edges->node = left_n->edges->node;
                if (cur_n->edges->node && cur_n->edges->node->data.value != 0)
                {
                    cur_n->edges->node->edge_inbound++;
                }
            }
        }
        if (right_n)
        {
            if (right_n->data.value == 0)
            {
                cur_n->edges->next->node = right_n->edges->node;
                if (cur_n->edges->next->node &&cur_n->edges->next->node->data.value != 0)
                {
                    cur_n->edges->next->node->edge_inbound++;
                }
            }
        }

        cur_n = cur_n->next;
    }

    cur_n = chems->chemicals_cur->nodes;
    while(cur_n)
    {
        if (cur_n->data.value == 0)
        {
            mov_n = cur_n;
            cur_n = cur_n->next;
            mov_n->next = NULL;
            
            if (!prev_n)
            {
                chems->chemicals_cur->nodes = cur_n;
            }
            else
            {
                prev_n->next = cur_n;
            }
            graph_edge_count_deduction(mov_n);
            free(mov_n);
        }
        else
        {
            prev_n = cur_n;
            cur_n = cur_n->next;
        }
    }
    chems->chemicals_cur->type = graph_evaluate(chems->chemicals_cur->nodes);
}

void remove_trash(struct chemicals *chems)
{
    struct _node *mov_n = NULL;
    struct _node *cur_n = chems->chemicals_cur->nodes;
    struct _node *prev_n = NULL;

    while(cur_n)
    {

        if ((cur_n->edges->next->node && !_graphFind(chems->chemicals_cur->nodes, cur_n->edges->next->node->data.value))
        || (cur_n->edges->node && !_graphFind(chems->chemicals_cur->nodes, cur_n->edges->node->data.value))
        || cur_n->edges->out_of_bounds || cur_n->edges->next->out_of_bounds)
        {
            mov_n = cur_n;
            cur_n = cur_n->next;
            mov_n->next = NULL;
            
            if (!prev_n)
            {
                chems->chemicals_cur->nodes = cur_n;
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
    chems->chemicals_cur->type = graph_evaluate(chems->chemicals_cur->nodes);
}

void remove_ammonia(struct chemicals *chems)
{
    struct _node *mov_n = NULL;
    struct _node *cur_n = chems->chemicals_cur->nodes;
    struct _node *prev_n = NULL;

    while(cur_n)
    {
        if (is_undulating(cur_n->data.value) && cur_n->data.value != 0)
        {
            mov_n = cur_n;
            cur_n = cur_n->next;
            mov_n->next = NULL;
            
            if (!prev_n)
            {
                chems->chemicals_cur->nodes = cur_n;
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

            graph_replace_edges(mov_n, chems->chemicals_cur->nodes);
            graph_edge_count_deduction(mov_n);
        }
        else
        {
            prev_n = cur_n;
            cur_n = cur_n->next;
        }
    }
    chems->chemicals_cur->type = graph_evaluate(chems->chemicals_cur->nodes);
}

void remove_feces(struct chemicals *chems)
{
    struct _node *mov_n = NULL;
    struct _node *cur_n = chems->chemicals_cur->nodes;
    struct _node *prev_n = NULL;

    while(cur_n)
    {
        if (is_prime(cur_n->data.value) && cur_n->data.value != 0)
        {
            mov_n = cur_n;
            cur_n = cur_n->next;
            mov_n->next = NULL;
            
            if (!prev_n)
            {
                chems->chemicals_cur->nodes = cur_n;
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

            graph_replace_edges(mov_n, chems->chemicals_cur->nodes);
            graph_edge_count_deduction(mov_n);
        }
        else
        {
            prev_n = cur_n;
            cur_n = cur_n->next;
        }
    }
    chems->chemicals_cur->type = graph_evaluate(chems->chemicals_cur->nodes);
}

struct _node *find_lowest_mercury(struct _node *n)
{
    struct _node *low_n = NULL;
    while(n)
    {
        if (n->edge_inbound == 0 && n->data.value != 0)
        {
            if (!low_n)
            {
                low_n = n;
            }
            else if (low_n->data.value > n->data.value)
            {
                low_n = n;
            }
        }
        n = n->next;
    }

    return low_n;
}

int remove_hazard(struct chemicals *chems)
{
    struct _node *mov_n = NULL;
    struct _node *cur_n = chems->chemicals_cur->nodes;
    struct _node *prev_n = NULL;

    mov_n = find_lowest_mercury(chems->chemicals_cur->nodes);
    if (!mov_n)
    {
        return 1;
    }

    while(cur_n)
    {
        if (cur_n->data.value != 0 && cur_n->data.value == mov_n->data.value)
        {
            cur_n = cur_n->next;
            mov_n->next = NULL;

            if (!prev_n)
            {
                chems->chemicals_cur->nodes = cur_n;
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
            break;
        }
        prev_n = cur_n;
        cur_n = cur_n->next;
    }

    chems->chemicals_cur->type = graph_evaluate(chems->chemicals_cur->nodes);

    return 0;
}

void set_total_sz(struct chemicals *chems)
{
    chems->total_sz = 0;
    for (unsigned int i = 1; i < chems->chemicals_sz; ++i)
    {
        if (chems->chemicals_g[i]->nodes)
        {
            graph_size(chems->chemicals_g[i]->nodes, &chems->total_sz);
        }
    }
    graph_size(chems->hazmat_g->nodes, &chems->total_sz);
    graph_size(chems->sludge_g->nodes, &chems->total_sz);
    graph_size(chems->trash_g->nodes, &chems->total_sz);
    graph_size(chems->chlorine_g->nodes, &chems->total_sz);
}

void free_chemicals(struct chemicals *chems)
{
    if (!chems)
    {
        return;
    }
    free_chem_idx(chems->chlorine);
    free(chems->chemicals);
    graphDestroy(chems->chemicals_cur);
    graphDestroy(chems->hazmat_g);
    graphDestroy(chems->sludge_g);
    graphDestroy(chems->trash_g);
    if (chems->sludge)
    {
        free(chems->sludge);
    }
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