
#ifndef listeners_H
#define listeners_H

struct session_data
{
    int sd;
    char *addr;
    unsigned int *liq_sz;
    unsigned int *chl_sz;
};

void *listener(void *data);
void *pretreatment(void *data);

#endif
