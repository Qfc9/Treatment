#ifndef RESPONSE_H
#define RESPONSE_H

#include "chemicals.h"

void send_downstream(struct chemicals *chems, unsigned int p);
void send_treatment(struct chemicals *chems);

#endif