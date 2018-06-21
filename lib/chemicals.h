
#ifndef CHEMICALS_H
#define CHEMICALS_H

struct chemicals
{
    struct molecule *chemicals;
    struct _graph *chemicals_g;
    uint32_t total_sz;
    uint32_t sz;
    uint16_t chlorine_sz;
    uint16_t chlorine_max;
    uint16_t chlorine_min;
    struct chemical_idx *chlorine;
    uint16_t air_sz;
    struct chemical_idx *air;
};

struct chemical_idx
{
    uint32_t idx;
    struct chemical_idx *next;
};


struct chemicals* analyze(struct molecule *m_buff, uint16_t sz);
int lead_detect(struct _node *n);
// void unchlorinate(struct chemicals *chems);
// void chlorinate(struct chemicals *chems);

void free_chemicals(struct chemicals *chems);

#endif
