#define _GNU_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "graph.h"
#include "chemicals.h"
#include "payloads.h"
#include "listener.h"
#include "response.h"

void *session(void *data);

// Opens a socket and listens for incoming connections, spins off new threads for new connections
void *listener(void *data)
{
    char port[5] = "3018";

    // Thread initiation
    pthread_attr_t  attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // Socket setup
    struct addrinfo hints = {0};
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;

    // Getting socket info
    struct addrinfo *results;
    int err = getaddrinfo(NULL, port, &hints, &results);
    if(err != 0) {
        // fprintf(stderr, "Could not get address: %s\n", gai_strerror(err));
        return NULL;
    }

    // Establishing the socket
    int sd = socket(results->ai_family, results->ai_socktype, 0);
    if(sd < 0) {
        perror("Could not establish socket");
        freeaddrinfo(results);
        return NULL;
    }

    // Adding socket option for faster binding
    int yes = 1;
    err = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if(err < 0) {
        perror("Unable to alter socket options");
        close(sd);
        return NULL;
    }

    // Binding to that socket
    err = bind(sd, results->ai_addr, results->ai_addrlen);
    if(err < 0) {
        perror("Could not bind socket");
        close(sd);
        freeaddrinfo(results);
        return NULL;
    }

    // Freeing the results
    freeaddrinfo(results);

    // Listening for connections
    err = listen(sd, 5);
    if(err < 0) {
        perror("Could not listen for connections");
        close(sd);
        return NULL;
    }

    pthread_t sessions;
    struct sessionData *sData;
    // Incoming loop
    while(1) {
        struct sockaddr_storage remote = {0};
        socklen_t remote_sz = sizeof(remote);

        // Accepting incoming
        int incoming = accept(sd, (struct sockaddr *)&remote, &remote_sz);
        if(incoming < 0) {
            perror("Could not accept incoming");
            continue;
        }

        printf("MAKING THREAD!\n");

        // Mallocing data for every thread
        sData = malloc(sizeof(struct sessionData));
        if (!sData)
        {
            perror("Could not malloc");
            continue;
        }
        sData->sd = incoming;

        // Creating session threads Thread
        pthread_create(&sessions, &attr, session, sData);

    }

    return data;
}

// Gets ran for every incoming connection
void *session(void *data)
{
    // Extracting the data and freeing
    int sd = ((struct sessionData *)data)->sd;
    free(data);

    // 65565
    struct header *head = calloc(1, 8);
    ssize_t sz = recv(sd, head, 8, 0);
    ssize_t tmpSz = 0;
    if (sz < 1)
    {
        close(sd);
        return NULL;
    }

    head->size = ntohs(head->size);

    struct molecule *m_buff = calloc(1, head->size - 8);
    sz = 0;
    sz = recv(sd, m_buff, head->size - 8, MSG_WAITALL);

    while(sz < (head->size - 8) && (tmpSz = recv(sd, m_buff + sz, head->size - 8, MSG_WAITALL)) > 0)
    {
        sz += tmpSz;
        tmpSz = 0;
    }

    struct chemicals *chems = analyze(m_buff, head->size - 8);
    printf("Total: %u\n", chems->sz/8);
    printf("CL: MAX: %u, MIN: %u, SZ: %u\n", chems->chlorine_max, chems->chlorine_min, chems->chlorine_sz);
    
    printf("RECIVED\n");
    graphPrint(chems->chemicals_g);

    while(chems->chemicals_g->type == GRAPH && lead_detect(chems->chemicals_g->nodes))
    {
        remove_lead(chems);
    }
    if (chems->hazmat_g->nodes)
    {
        printf("LEAD:\n");
        graphPrint(chems->hazmat_g);
    }

    remove_feces(chems);
    if (chems->sludge_g->nodes)
    {
        printf("SLUDGE:\n");
        graphPrint(chems->sludge_g);
        sludgified(chems);
        send_downstream(chems, 4);
    }

    printf("SENDING\n");
    if (chems->hazmat_g->nodes)
    {
        chems->sz = graph_payload(chems->hazmat_g);
        send_downstream(chems, 8);
    }

    chems->sz = graph_payload(chems->chemicals_g);

    graphPrint(chems->chemicals_g);

    send_downstream(chems, 1);

    free(head);
    free_chemicals(chems);
    
    // Closing the connection
    close(sd);
    return NULL;
}

