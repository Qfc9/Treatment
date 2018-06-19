
#ifndef CHEMICALS_H
#define CHEMICALS_H

struct chemicals
{
    char *chemicals;
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


struct chemicals* analyze(char *data, uint16_t sz);
void unchlorinate(struct chemicals *chems);
void chlorinate(struct chemicals *chems);
void deaerate(struct chemicals *chems);

void free_chemicals(struct chemicals *chems);

#endif
