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

#include "chemicals.h"
#include "payloads.h"
#include "listener.h"
#include "response.h"

void *session(void *data);

// Opens a socket and listens for incoming connections, spins off new threads for new connections
void *listener(void *data)
{
    char port[5] = "1111";

    // Thread initiation
    pthread_attr_t  attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // Socket setup
    struct addrinfo hints = {0};
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Getting socket info
    struct addrinfo *results;
    int err = getaddrinfo(NULL, port, &hints, &results);
    if(err != 0) {
        fprintf(stderr, "Could not get address: %s\n", gai_strerror(err));
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

    return NULL;
}

// Gets ran for every incoming connection
void *session(void *data)
{
    // Extracting the data and freeing
    int sd = ((struct sessionData *)data)->sd;
    free(data);

    // 65565
    struct header *head = calloc(1, sizeof(struct head *));
    int sz = recv(sd, head, sizeof(*head), 0);
    if (sz < 1)
    {
        close(sd);
        return NULL;
    }

    char *buff = calloc(1, head->size - 8);
    sz = recv(sd, buff, head->size - 8, 0);
    if (sz < 1)
    {
        close(sd);
        return NULL;
    }

    struct chemicals *chems = analyze(buff, head->size - 8);
    printf("CL: MAX: %zu, SZ: %zu\n", chems->chlorine_max, chems->chlorine_sz);
    printf("AIR: SZ: %zu\n", chems->air_sz);
    
    if (chems->air_sz > 0)
    {
        deaerate(buff, chems);
    }
    if (chems->chlorine_sz > chems->chlorine_max)
    {
        unchlorinate(buff, chems);
    }

    struct molecule m;
    for (int i = 0; i < sz / 8; ++i)
    {   
        memcpy(&m, &buff[i*8], 8);

        printf("Data: %u\n", m.data);
        printf("Left: %u\n", m.left);
        printf("Right: %u\n\n", m.right);
    }

    // Handling the data sent
    // send_downstream(buff, head);

    free(buff);
    free(head);
    free_chemicals(chems);
    
    // Closing the connection
    close(sd);
    return NULL;
}

