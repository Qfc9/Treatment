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
    chems->air_sz = 0;
    chems->air = NULL;

    chems->chlorine_max = (unsigned int)((sz / 8) * 0.05);

    struct molecule m;
    for (uint16_t i = 0; i < sz / 8; ++i)
    {   
        memcpy(&m, &data[i*8], 8);

        if (m.data == 0)
        {
            chems->air_sz++;
            struct chemical_idx *air = calloc(1, sizeof(struct chemical_idx));
            air->next = NULL;
            air->idx = i;

            if (!chems->air)
            {
                chems->air = air;
            }
            else
            {
                add_chemical(chems->air, air);
            }
        }
        else if (m.left == m.right && m.left != 0 && m.right != 0)
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

void unchlorinate(char *data, struct chemicals *chems)
{
    // struct molecule m;

    struct chemical_idx *chlorine = chems->chlorine;

    while(chlorine && (chems->chlorine_sz > chems->chlorine_max))
    {
        data[(chlorine->idx * 8) + 6] = 0;
        data[(chlorine->idx * 8) + 7] = 0;
        chlorine = chlorine->next;
        printf("AFSDF\n");
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