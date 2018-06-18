
#ifndef CHEMICALS_H
#define CHEMICALS_H

struct chemicals
{
    uint16_t chlorine_sz;
    uint16_t chlorine_max;
    struct chemical_idx *chlorine;
};

struct chemical_idx
{
    uint32_t idx;
    struct chemical_idx *next;
};


struct chemicals* analyze(char *data, uint16_t sz);
void unchlorinate(char *data, uint16_t sz);

void free_chemicals(struct chemicals *chems);

#endif
