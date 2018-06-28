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

    unsigned int sz;
    void *payload;
    char port[5];
    struct header head = {0};

    switch(p)
    {
        case 2:
            strncpy(port, "2222", 5);
            payload = chems->trash_g->payload;
            sz = chems->trash_g->sz;
            head.type = TRASH;
            break;
        case 4:
            strncpy(port, "4444", 5);
            payload = chems->sludge;
            sz = chems->sludge_g->sz;
            head.type = SLUDGE;
            break;
        case 8:
            strncpy(port, "8888", 5);
            payload = chems->hazmat_g->payload;
            sz = chems->hazmat_g->sz;
            head.type = HAZMAT;
            break;
        case 9:
            strncpy(port, "9999", 5);
            payload = NULL;
            sz = 64;
            head.type = REPORTING;
            break;
        default:
            strncpy(port, "1111", 5);
            payload = chems->liquid_g->payload;
            sz = chems->liquid_g->sz;
            head.type = WASTEWATER;
            break;
    }

    head.size = htons(sz + 8);
    head.type = htons(head.type);

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

    send(sd, &head, sizeof(head), 0);
    if (payload)
    {
        send(sd, payload, sz, 0);
    }
    else
    {
        send(sd, &chems->report, 8, 0);
        send(sd, chems->report.message, 56, 0);
    }

    close(sd);
}

void send_treatment(struct chemicals *chems)
{
    char port[5] = "1111";

    // Socket setup
    struct addrinfo hints = {0};
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;

    // Getting socket info
    struct addrinfo *results;
    int err = getaddrinfo("10.40.18.1", port, &hints, &results);
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

    struct header head = {0, chems->liquid_g->sz + 8, 0};
    head.size = htons(head.size);
    send(sd, &head, sizeof(head), 0);
    send(sd, chems->liquid_g->payload, chems->liquid_g->sz, 0);

    close(sd);  
}
