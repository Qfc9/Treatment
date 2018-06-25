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

#include "../lib/graph.h"
#include "../lib/chemicals.h"
#include "../lib/payloads.h"
#include "../lib/listener.h"
#include "../lib/response.h"

void *pretreatment(void *data);

int main(void)
{
    char done[16];

    // Thread init
    pthread_t thread;
    pthread_attr_t  attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&thread, &attr, listener, pretreatment);

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

// Gets ran for every incoming connection
void *pretreatment(void *data)
{
    // Extracting the data and freeing
    int sd = ((struct session_data *)data)->sd;
    void *func = ((struct session_data *)data)->func;
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
    printf("RECIVED\n");
    graphPrint(chems->chemicals_g);

    if (chems->chemicals_g->type == GRAPH)
    {
        analyze_hazmat(chems);
    }

    while(chems->chemicals_g->type == GRAPH)
    {
        if (chems->hazmat_sz == 1)
        {
            remove_lead(chems);
        }
        else if (chems->hazmat_sz > 1)
        {
            remove_mercury(chems);
        }
        else
        {
            break;
        }
    }

    if (chems->hazmat_g->nodes)
    {
        printf("SENDING HAZMAT\n");
        chems->sz = graph_payload(chems->hazmat_g);
        graphPrint(chems->hazmat_g);
        send_downstream(chems, 8);
    }

    if (chems->chemicals_g->nodes)
    {
        printf("SENDING DOWNSTREAM\n");
        chems->sz = graph_payload(chems->chemicals_g);
        graphPrint(chems->chemicals_g);
        send_treatment(chems);
    }

    free(head);
    free_chemicals(chems);
    
    // Closing the connection
    close(sd);
    return NULL;
}