#define _XOPEN_SOURCE 600

#include <signal.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../lib/payloads.h"

int main(void)
{
    struct header head = {0, 8 * 101, 0};
    struct molecule m[100];

    for (unsigned int i = 0; i < 100; ++i)
    {
        m[i].data = 1;
        m[i].left = 1;
        m[i].right = 2;
    }

    // m[0].data = 1;
    // m[0].left = 2;
    // m[0].right = 0;


    // m[1].data = 0;
    // m[1].left = 1;
    // m[1].right = 2;


    // m[2].data = 3;
    // m[2].left = 0;
    // m[2].right = 0;


    // m[3].data = 4;
    // m[3].left = 2;
    // m[3].right = 2;

    // m[4].data = 2;
    // m[4].left = 4;
    // m[4].right = 3;

    char port[5] = "1111";

    // Socket setup
    struct addrinfo hints = {0};
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Getting socket info
    struct addrinfo *results;
    int err = getaddrinfo("10.40.18.130", port, &hints, &results);
    if(err != 0) {
        fprintf(stderr, "Could not get address: %s\n", gai_strerror(err));
        return 1;
    }

    // Establishing the socket
    int sd = socket(results->ai_family, results->ai_socktype, 0);
    if(sd < 0) {
        perror("Could not establish socket");
        freeaddrinfo(results);
        return 1;
    }

    // Adding socket option for faster binding
    int yes = 1;
    err = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if(err < 0) {
        perror("Unable to alter socket options");
        close(sd);
        return 1;
    }

    // Attempting to connect to dispatcher
    err = connect(sd, results->ai_addr, results->ai_addrlen);
    if(err < 0)
    {
        perror("Could not bind socket");
        close(sd);
        freeaddrinfo(results);
        return 1;
    }

    // Freeing the results
    freeaddrinfo(results);

    send(sd, &head, 8, 0);
    send(sd, m, 100 * 8, 0);

    for (int i = 0; i < 100; ++i)
    {
        printf("Data: %u\n", m[i].data);
        printf("Left: %u\n", m[i].left);
        printf("Right: %u\n\n", m[i].right);
    }

    close(sd);

    return 0;
}