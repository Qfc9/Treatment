#include <stdio.h>
#include <stdlib.h>

#include "../lib/util.h"

int main(int argc, char *argv[])
{
    uint32_t num = 0;
    sscanf(argv[1], "%u", &num);
    printf("DATA: %u\n", num);
    if(is_undulating(num))
    // if(is_prime(num))
    {
        printf("YES\n");
    }
    else
    {
        printf("NO\n");
    }
    return 0;
}