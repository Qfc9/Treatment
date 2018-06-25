#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>

#include "util.h"

bool lesser_greater(char *n);
bool greater_lesser(char *n);

bool is_undulating(uint32_t n)
{
    char number[16];
    snprintf(number, 16, "%u", n);

    if (number[0] > number[1])
    {
        return greater_lesser(number);
    }
    else if (number[0] < number[1])
    {
        return lesser_greater(number);
    }

    return false;
}

bool greater_lesser(char *n)
{
    ssize_t sz = strlen(n);

    for (ssize_t i = 1; i < sz; ++i)
    {
        if (n[i] == n[i - 1])
        {
            return false;
        }
        else if (n[i] > n[i - 1] && (i % 2 != 0))
        {
            return false;
        }
        else if (n[i] < n[i - 1] && (i % 2 == 0))
        {
            return false;
        }
    }

    return true;
}

bool lesser_greater(char *n)
{
    ssize_t sz = strlen(n);
   
    for (ssize_t i = 1; i < sz; ++i)
    {
        if (n[i] == n[i - 1])
        {
            return false;
        }
        else if (n[i] < n[i - 1] && (i % 2 != 0))
        {
            return false;
        }
        else if (n[i] > n[i - 1] && (i % 2 == 0))
        {
            return false;
        }
    }

    return true;
}

bool is_prime(uint32_t n)
{
    if (n == 0 || n == 1)
    {
        return false;
    }
    else if(n == 2)
    {
        return true;
    }
    else if(n % 2 == 0)
    {
        return false;
    }

    double num = n;
    double sqrt_max = floor(sqrt(num));

    uint32_t max = sqrt_max;

    for (uint32_t i = 1; i <= max; i += 2)
    {
        if (i == 1)
        {
            continue;
        }

        if ((n % i) == 0)
        {
            return false;
        }
    }
    return true;
}