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
#include <sys/stat.h>

#include <ifaddrs.h>
#include <netinet/in.h>

#include "util.h"
#include "listener.h"

// Opens a socket and listens for incoming connections, spins off new threads for new connections
void *listener(void *data)
{
    char port[5];

    if (strncmp("eharmon", getlogin(), 7) == 0)
    {
        strncpy(port, "3018", 5);
    }
    else
    {
        strncpy(port, "1111", 5);
    }

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
    struct session_data *s_data;
    unsigned int liq_sz = 0;
    unsigned int chl_sz = 0;
    // Incoming loop
    while(1) {
        printf("LIQ: %u CHL: %u\n", liq_sz, chl_sz);
        struct sockaddr_storage remote = {0};
        socklen_t remote_sz = sizeof(remote);

        // Accepting incoming
        int incoming = accept(sd, (struct sockaddr *)&remote, &remote_sz);
        if(incoming < 0) {
            perror("Could not accept incoming");
            continue;
        }

        char *addr = calloc(1, 16);

        if(remote.ss_family == AF_INET) {
            inet_ntop(remote.ss_family, &((struct sockaddr_in *)&remote)->sin_addr, addr, 16);
            printf("Received from %s\n", addr);
        } 

        printf("MAKING THREAD!\n");

        // Mallocing data for every thread
        s_data = malloc(sizeof(struct session_data));
        if (!s_data)
        {
            perror("Could not malloc");
            continue;
        }

        s_data->sd = incoming;
        s_data->addr = addr;
        s_data->liq_sz = &liq_sz;
        s_data->chl_sz = &chl_sz;

        // Creating session threads Thread
        pthread_create(&sessions, &attr, data, s_data);

    }

    return NULL;
}
