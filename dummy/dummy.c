#define _XOPEN_SOURCE 600
#define PAYLOAD_SIZE 52

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
    struct header head = {0, 8 * (PAYLOAD_SIZE + 1), 0};

    // for (unsigned int i = 0; i < PAYLOAD_SIZE; ++i)
    // {
    //     m[i].data = htonl(i);
    //     m[i].left = htons(1);
    //     m[i].right = htons(1);
    // }

    struct molecule m[PAYLOAD_SIZE] = {
        // {{htonl(437472611), htons(6), htons(0)},
        // {htonl(2595747122), htons(0), htons(0)},
        // {htonl(3), htons(2), htons(5)},
        // {htonl(2595747120), htons(2), htons(1)},
        // {htonl(0), htons(0), htons(0)},
        // {htonl(0), htons(2), htons(4)},
        // {htonl(0), htons(3), htons(0)}};

{htonl(158826186), htons(0), htons(0 )},
{htonl(2102341165), htons(7), htons(7 )},
{htonl(0), htons(0), htons(0 )},
{htonl(2323919844), htons(51), htons(51 )},
{htonl(197313560), htons(43), htons(43 )},
{htonl(0), htons(0), htons(0 )},
{htonl(3163465641), htons(9), htons(9 )},
{htonl(2299404264), htons(7), htons(7 )},
{htonl(1850636209), htons(43), htons(43 )},
{htonl(1665684734), htons(1), htons(1 )},
{htonl(2500323947), htons(7), htons(7 )},
{htonl(0), htons(0), htons(0 )},
{htonl(3980119256), htons(9), htons(9 )},
{htonl(2765015452), htons(26), htons(26 )},
{htonl(1742567042), htons(7), htons(7 )},
{htonl(2611217597), htons(7), htons(7 )},
{htonl(0), htons(0), htons(0 )},
{htonl(3756528821), htons(49), htons(49 )},
{htonl(899834659), htons(49), htons(49 )},
{htonl(0), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(4237505087), htons(43), htons(43 )},
{htonl(0), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(664177049), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(2754869304), htons(49), htons(49 )},
{htonl(1344601859), htons(7), htons(7 )},
{htonl(843043261), htons(49), htons(49 )},
{htonl(0), htons(0), htons(0 )},
{htonl(3866189308), htons(7), htons(7 )},
{htonl(2545390462), htons(1), htons(1 )},
{htonl(1086671540), htons(4), htons(4 )},
{htonl(447191802), htons(7), htons(7 )},
{htonl(0), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(953425750), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(0), htons(0), htons(0 )},
{htonl(2386866943), htons(43), htons(43 )},
{htonl(3844059643), htons(49), htons(49 )},
{htonl(0), htons(0), htons(0 )},
{htonl(2030246636), htons(26), htons(26 )},
{htonl(0), htons(0), htons(0 )},
{htonl(3797359778), htons(7), htons(7 )},
{htonl(0), htons(0), htons(0 )}
};

    char port[5] = "3018";

    // Socket setup
    struct addrinfo hints = {0};
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Getting socket info
    struct addrinfo *results;
    int err = getaddrinfo(NULL , port, &hints, &results);
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

    head.size = htons(head.size);
    send(sd, &head, 8, 0);
    send(sd, m, PAYLOAD_SIZE * 8, 0);

    for (int i = 0; i < PAYLOAD_SIZE; ++i)
    {
        printf("Data: %u\n", m[i].data);
        printf("Left: %u\n", m[i].left);
        printf("Right: %u\n\n", m[i].right);
    }

    close(sd);

    return 0;
}