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

#include "../lib/graph.h"
#include "../lib/chemicals.h"
#include "../lib/payloads.h"
#include "../lib/listener.h"
#include "../lib/response.h"
#include "../lib/crypt.h"
#include "../lib/util.h"
#include "../lib/reporting.h"

void *session(void *data);

int main(void)
{
    // Thread init
    pthread_t thread;
    pthread_attr_t  attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&thread, &attr, listener, session);

    // Printing and waiting for an enter to end
    printf("Server Online...\n");
    printf("Press enter to close the server...\n");
    getc(stdin);

    // Killing all the threads
    pthread_kill(thread, SIGINT);

    // Freeing
    pthread_attr_destroy(&attr);

    return 0;
}

// Gets ran for every incoming connection
void *session(void *data)
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

    struct chemicals *chems = create_chemicals(m_buff, head->size - 8);

    if ((head->size - 8) != sz)
    {
        report_invalid_sz(chems, addr);
    }
    else
    {
        printf("Total: %u\n", chems->sz/8);    
        printf("RECIVED\n");
        graphPrint(chems->chemicals_g[0]);

        break_up_compounds(chems);
        analyze(chems);

        for (unsigned int i = 1; i < chems->chemicals_sz; ++i)
        {
            chems->chemicals_cur = chems->chemicals_g[i];

            if (chems->chemicals_cur->type == GRAPH)
            {
                analyze_hazmat(chems);
            }

            while(chems->chemicals_cur->type == GRAPH)
            {
                if(remove_hazard(chems) == 1)
                {
                    break;
                }
            }
            remove_feces(chems);
            remove_ammonia(chems);
            remove_air(chems);

            while(trash_detect(chems))
            {
                remove_trash(chems);
            }

            chlorine_detect(chems);
        }

        struct _node *end_n = NULL;
        for (unsigned int i = 1; i < chems->chemicals_sz; ++i)
        {
            if (chems->chemicals_g[i]->nodes)
            {
                if (!chems->liquid_g->nodes)
                {
                    chems->liquid_g->nodes = chems->chemicals_g[i]->nodes;
                    end_n = chems->liquid_g->nodes;
                }
                else
                {
                    while(end_n->next)
                    {
                        end_n = end_n->next;
                    }
                    end_n->next = chems->chemicals_g[i]->nodes;
                }
            }
        }

        if (chems->hazmat_g->nodes)
        {
            printf("HAZMAT:\n");
            graphPrint(chems->hazmat_g);
        }
        if (chems->sludge_g->nodes)
        {
            printf("SLUDGE:\n");
            graphPrint(chems->sludge_g);
        }
        if (chems->trash_g->nodes)
        {
            printf("TRASH:\n");
            graphPrint(chems->trash_g);
        }
        if (chems->liquid_g->nodes)
        {
            printf("LIQUID:\n");
            graphPrint(chems->liquid_g);
        }

        illegal_detect(chems, addr);

        graph_payload(chems->trash_g);
        sludgified(chems);
        graph_payload(chems->hazmat_g);
        graph_payload(chems->liquid_g);

        if (chems->trash_g->payload)
        {
            printf("SENDING TRASH\n");
            send_downstream(chems, 2);
        }
        if (chems->sludge)
        {
            printf("SENDING SLUDGE\n");
            send_downstream(chems, 4);
        }
        if (chems->hazmat_g->payload)
        {
            printf("SENDING HAZMAT\n");
            send_downstream(chems, 8);
        }
        if (chems->liquid_g->payload)
        {
            printf("SENDING LIQUID\n");
            send_downstream(chems, 1);
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