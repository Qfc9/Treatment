
#ifndef CHEMICALS_H
#define CHEMICALS_H

#include "payloads.h"

struct chemicals
{
    struct molecule *chemicals;
    struct _graph *chemicals_g;
    struct _graph *hazmat_g;
    struct _graph *sludge_g;
    struct _graph *trash_g;
    struct _graph *chlorine_g;
    uint8_t *sludge;
    struct report report;
    uint32_t total_sz;
    uint32_t sz;
    uint32_t hazmat_sz;
    uint32_t trash_sz;
    uint32_t chlorine_sz;
    uint16_t chlorine_max;
    uint16_t chlorine_min;
    struct chemical_idx *chlorine;
};

struct chemical_idx
{
    uint32_t idx;
    struct chemical_idx *next;
};


struct chemicals* analyze(struct molecule *m_buff, uint16_t sz);
int lead_detect(struct _node *n);
void illegal_detect(struct chemicals *chems, char *addr);
void chlorine_detect(struct chemicals *chems);
void analyze_hazmat(struct chemicals *chems);
int trash_detect(struct chemicals *chems);
void remove_feces(struct chemicals *chems);
int remove_hazard(struct chemicals *chems);
void remove_ammonia(struct chemicals *chems);
void remove_air(struct chemicals *chems);
void remove_trash(struct chemicals *chems);

void set_total_sz(struct chemicals *chems);

void add_chlorine(struct chemicals *chems, unsigned int *liq_sz, unsigned int *chl_sz);

void free_chemicals(struct chemicals *chems);

#endif