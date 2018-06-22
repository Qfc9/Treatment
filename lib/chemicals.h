
#ifndef CHEMICALS_H
#define CHEMICALS_H

struct chemicals
{
    struct molecule *chemicals;
    struct _graph *chemicals_g;
    struct _graph *hazmat_g;
    struct _graph *sludge_g;
    struct _graph *trash_g;
    struct sludge *sludge;
    uint32_t total_sz;
    uint32_t sz;
    uint16_t chlorine_sz;
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
int trash_detect(struct chemicals *chems);
void remove_feces(struct chemicals *chems);
void remove_lead(struct chemicals *chems);
void remove_ammonia(struct chemicals *chems);
void remove_trash(struct chemicals *chems);
void sludgified(struct chemicals *chems);

void free_chemicals(struct chemicals *chems);

#endif
