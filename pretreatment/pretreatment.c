#define _XOPEN_SOURCE 600
#define _WITH_DPRINTF

#include <signal.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../lib/listener.h"

int main(void)
{
    char done[16];

    // Thread init
    pthread_t thread;
    pthread_attr_t  attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&thread, &attr, listener, NULL);

    // Printing and waiting for an enter to end
    printf("Server Online...\n");
    printf("Press enter to close the server...\n");
    scanf("%s", done);

    // Killing all the threads
    pthread_kill(thread, SIGINT);

    // Freeing
    pthread_attr_destroy(&attr);

    return 0;
}