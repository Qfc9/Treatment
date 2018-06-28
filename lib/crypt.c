#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>

#include "libscrypt/libscrypt.h"
#include "graph.h"
#include "payloads.h"
#include "chemicals.h"
#include "util.h"

// void sludgified(struct chemicals *chems)
// {
//     uint32_t size = 0;
//     graph_size(chems->sludge_g->nodes, &size);

//     struct sludge *s = calloc(sizeof(*s), size);
//     struct _node *n = chems->sludge_g->nodes;

//     char input[16];

//     for (unsigned int i = 0; i < size; ++i)
//     {
//         snprintf(input, sizeof(input), "%u", n->data.value);
//         libscrypt_scrypt((const uint8_t *)input, strlen(input), (const uint8_t *)salt, strlen(salt), 2048, 4, 4, (uint8_t *) &s[i].hash, 64);
//         n = n->next;
//     }

//     chems->sz = size * 64;

//     chems->sludge = s;
// }

void sludgified(struct chemicals *chems)
{
    if (!chems->sludge_g->nodes)
    {
        return;
    }

    uint32_t size = 0;
    graph_size(chems->sludge_g->nodes, &size);

    if (size == 0)
    {
        return;
    }

    chems->sludge_g->sz = size * 64;

    uint8_t *hash = calloc(1, chems->sludge_g->sz);
    struct _node *n = chems->sludge_g->nodes;

    char input[16];

    for (unsigned int i = 0; i < size; ++i)
    {
        snprintf(input, sizeof(input), "%u", n->data.value);
        libscrypt_scrypt((const uint8_t *)input, strlen(input), (const uint8_t *)salt, strlen(salt), 2048, 4, 4, hash + (i * 64), 64);
        n = n->next;
    }

    chems->sludge = hash;
}