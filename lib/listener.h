
#ifndef listeners_H
#define listeners_H

struct session_data
{
    int sd;
    void *func;
};

void *listener(void *data);
void *pretreatment(void *data);

#endif
