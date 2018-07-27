
#ifndef REPORTING_H
#define REPORTING_H

#include "chemicals.h"

void report_invalid_sz(struct chemicals *chems, char *addr);
void report_illegal_dumping(struct chemicals *chems, char *addr, char *items);

#endif