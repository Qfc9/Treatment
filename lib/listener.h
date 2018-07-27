
#ifndef listeners_H
#define listeners_H

struct session_data
{
    int sd;
    char *addr;
};

void *listener(void *data);
void *pretreatment(void *data);

#endif
