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
#include "../lib/util.h"

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
    char *addr = ((struct session_data *)data)->addr;
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

    if ((head->size - 8) != sz)
    {
        char *dec_addr = ip_str_to_dec(addr);

        struct report report;
        report.error_type = htons(NOT_ENOUGH_DATA);
        report.custom = 0;
        report.ip_addr = htonl(*dec_addr);
        free(dec_addr);

        snprintf(report.message, 56, "INVALID SIZE FROM: %s", addr);
        printf("%s\n", report.message);

        chems->sz = 64;

        send_downstream(chems, 9);
    }
    else
    {
        printf("Total: %u\n", chems->sz/8);    
        printf("RECIVED\n");
        graphPrint(chems->chemicals_g);

        if (chems->chemicals_g->type == GRAPH)
        {
            analyze_hazmat(chems);
        }

        while(chems->chemicals_g->type == GRAPH)
        {
            if(remove_hazard(chems) == 1)
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
            printf("SENDING LIQUID\n");
            chems->sz = graph_payload(chems->chemicals_g);
            graphPrint(chems->chemicals_g);
            send_treatment(chems);
        }
    }

    if (addr)
    {
        free(addr);
    }
    free(head);
    free_chemicals(chems);
    
    // Closing the connection
    close(sd);
    return NULL;
}
