#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdint.h>

bool is_prime(uint32_t n);
bool is_undulating(uint32_t n);
uint32_t *ip_str_to_dec(char *ip_str);

#endif