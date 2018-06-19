
#ifndef CHEMICALS_H
#define CHEMICALS_H

struct chemicals
{
    uint16_t chlorine_sz;
    uint16_t chlorine_max;
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
void unchlorinate(char *data, struct chemicals *chems);

void free_chemicals(struct chemicals *chems);

#endif
