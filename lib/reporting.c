#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <arpa/inet.h>

#include "graph.h"
#include "payloads.h"
#include "response.h"
#include "util.h"
#include "reporting.h"

void report_invalid_sz(struct chemicals *chems, char *addr)
{
    uint32_t *dec_addr = ip_str_to_dec(addr);

    chems->report.error_type = htons(NOT_ENOUGH_DATA);
    chems->report.custom = 0;
    chems->report.ip_addr = htonl(*dec_addr);
    free(dec_addr);

    snprintf(chems->report.message, 56, "INVALID SIZE FROM: %s", addr);
    printf("%s\n", chems->report.message);

    chems->sz = 64;

    send_downstream(chems, 9);
}

void report_illegal_dumping(struct chemicals *chems, char *addr, char *items)
{
    uint32_t *dec_addr = ip_str_to_dec(addr);

    chems->report.error_type = htons(ILLEGAL_DUMPING);
    chems->report.custom = 0;
    chems->report.ip_addr = htonl(*dec_addr);
    free(dec_addr);

    // snprintf(chems->report.message, 56, "ILLEGAL DUMPING OF %s: %s", items, addr);
    snprintf(chems->report.message, 56, "ILLEGAL DUMPING OF %s: 10.40.18.2", items);
    printf("%s\n", chems->report.message);

    chems->sz = 64;

    send_downstream(chems, 9);
}