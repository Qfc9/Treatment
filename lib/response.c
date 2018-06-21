#define _GNU_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "graph.h"
#include "chemicals.h"
#include "payloads.h"
#include "response.h"

void send_downstream(struct chemicals *chems, unsigned int p)
{

    unsigned int type;
    char port[5];

    switch(p)
    {
        case 8:
            strncpy(port, "8888", 5);
            type = HAZMAT;
            break;
        case 4:
            strncpy(port, "4444", 5);
            type = SLUDGE;
            break;
        default:
            strncpy(port, "1111", 5);
            type = WASTEWATER;

    }
    // Socket setup
    struct addrinfo hints = {0};
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Getting socket info
    struct addrinfo *results;
    int err = getaddrinfo("10.40.18.151", port, &hints, &results);
    if(err != 0) {
        // fprintf(stderr, "Could not get address: %s\n", gai_strerror(err));
        return;
    }

    // Establishing the socket
    int sd = socket(results->ai_family, results->ai_socktype, 0);
    if(sd < 0) {
        perror("Could not establish socket");
        freeaddrinfo(results);
        return;
    }

    // Adding socket option for faster binding
    int yes = 1;
    err = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if(err < 0) {
        perror("Unable to alter socket options");
        close(sd);
        return;
    }

    // Attempting to connect to dispatcher
    err = connect(sd, results->ai_addr, results->ai_addrlen);
    if(err < 0)
    {
        perror("Could not bind socket");
        close(sd);
        freeaddrinfo(results);
        return;
    }

    // Freeing the results
    freeaddrinfo(results);

    struct header head = {type, chems->sz + 8, 0};
    head.size = htons(head.size);
    head.type = htons(head.type);
    if (p == 8)
    {
        send(sd, &head, sizeof(head), 0);
        send(sd, &chems->hazmat_g->payload, chems->sz, 0);
    }
    else if(p == 4)
    {
        send(sd, &head, sizeof(head), 0);
        send(sd, &chems->sludge, chems->sz, 0);
    }
    else
    {
        send(sd, &head, sizeof(head), 0);
        send(sd, &chems->chemicals_g->payload, chems->sz, 0);
    }

    close(sd);
}