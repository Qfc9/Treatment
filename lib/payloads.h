
#ifndef PAYLOADS_H
 #define PAYLOADS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

extern const char *salt;

enum type {wastewater=0, trash=1, sludge=2, hazmat=4, reporting=8};
enum error_type {RESERVED = 0, not_enough_data=8, illegal_dumping=16};

struct header {
    uint16_t type;
    uint16_t size;
    uint32_t custom;
};

struct report
{
    uint16_t error_type;
    uint16_t custom;
    uint32_t ip_addr;
    char message[56];
};

struct hz_container
{
    uint32_t data;
    uint32_t custom;
};

struct molecule
{
    uint32_t data;
    uint16_t left;
    uint16_t right;
};

struct sludge {
    uint8_t hash[64];
};

#endif
