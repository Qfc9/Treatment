#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>

#include "graph.h"
#include "payloads.h"
#include "chemicals.h"

void free_chem_idx(struct chemical_idx *chem);
void add_chemical(struct chemical_idx *chems, struct chemical_idx *new_chem);

struct chemicals* analyze(struct molecule *m_buff, uint16_t sz)
{
    struct chemicals *chems = calloc(1, sizeof(*chems));
    chems->chlorine = NULL;
    chems->chlorine_sz = 0;
    chems->total_sz = sz;
    chems->sz = sz;
    chems->chemicals = m_buff;
    chems->chemicals_g = graphCreate();

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

        if (m.data != 0)
        {
            graphAddNode(chems->chemicals_g, m.data);
        }

        if (m.left == m.right && m.left != 0 && m.right != 0 && m.data != 0)
        {
            chems->chlorine_sz++;
            struct chemical_idx *chlorine = calloc(1, sizeof(*chlorine));
            chlorine->next = NULL;
            chlorine->idx = m.data;

            if (!chems->chlorine)
            {
                chems->chlorine = chlorine;
            }
            else
            {
                add_chemical(chems->chlorine, chlorine);
            }
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
            continue;
        }

        if (m.left != 0)
        {
            mLink = m_buff[(m.left - 1)];
            mLink.data = ntohl(mLink.data);
            mLink.left = ntohs(mLink.left);
            mLink.right = ntohs(mLink.right);

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

void chlorinate(struct chemicals *chems)
{
    struct molecule chlorine = {2, 1, 1};
    printf("ADD CLORINE\n");

    while(chems->chlorine_sz < chems->chlorine_min)
    {
        if (chems->sz >= chems->total_sz)
        {
            chems->total_sz *= 2;
            void *tmp = realloc (chems->chemicals, chems->total_sz);
            if(!tmp)
            {
                return;
            }
            chems->chemicals = tmp;
        }

        chlorine.data = htonl(chlorine.data);
        chlorine.right = htons(chlorine.right);
        chlorine.left = htons(chlorine.left);
        memcpy(&chems->chemicals[chems->sz], &chlorine, 8);
        chems->sz += 8;
        chems->chlorine_sz++;

    }
}

void unchlorinate(struct chemicals *chems)
{
    struct chemical_idx *chlorine = chems->chlorine;
    printf("TAKE CLORINE\n");

    while(chlorine && (chems->chlorine_sz > chems->chlorine_max))
    {
        // chems->chemicals[(chlorine->idx * 8) + 6] = 0;
        // chems->chemicals[(chlorine->idx * 8) + 7] = 0;
        chlorine = chlorine->next;
    }
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