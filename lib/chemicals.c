#include <stdio.h>
#include <string.h>

#include "payloads.h"
#include "chemicals.h"

void free_chem_idx(struct chemical_idx *chem);
void add_chemical(struct chemical_idx *chems, struct chemical_idx *new_chem);

struct chemicals* analyze(char *data, uint16_t sz)
{
    struct chemicals *chems = calloc(1, sizeof(*chems));
    chems->chlorine = NULL;
    chems->chlorine_max = 0;
    chems->chlorine_sz = 0;

    chems->chlorine_max = (unsigned int)((sz / 8) * 0.05);

    struct molecule m;
    unsigned int max_cl = (unsigned int)((sz / 8) * 0.05);
    for (uint16_t i = 0; i < sz / 8; ++i)
    {   
        memcpy(&m, &data[i*8], 8);

        if (m.left == m.right && m.left != 0 && m.right != 0)
        {
            chems->chlorine_sz++;
            struct chemical_idx *chlorine = calloc(1, sizeof(struct chemical_idx));
            chlorine->next = NULL;
            chlorine->idx = i;

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

    return chems;
}

void add_chemical(struct chemical_idx *chems, struct chemical_idx *new_chem)
{
    return;
}

void unchlorinate(char *data, uint16_t sz)
{
    struct molecule m;
    unsigned int max_cl = (unsigned int)((sz / 8) * 0.05);

    for (uint16_t i = 0; i < sz / 8; ++i)
    {   
        memcpy(&m, &data[i*8], 8);

        if (m.left == m.right && m.left != 0)
        {
            if (max_cl > 0)
            {
                max_cl--;
            }
            else
            {
                m.right = 0;
                memcpy(&data[i*8], &m, 8);
            }
        }
    }
}

void free_chemicals(struct chemicals *chems)
{
    if (!chems)
    {
        return;
    }
    free_chem_idx(chems->chlorine);
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