
#ifndef listeners_H
#define listeners_H

struct listenerData
{
    char *port;
    char *www;
    char *cgi;
    char *err;
} listenerData;

struct sessionData
{
    int sd;
    char *www;
    char *cgi;
    char *err;
};

void *listener(void *data);
void *pretreatment(void *data);

#endif
