#include <math.h>

#include "util.h"

bool is_prime(unsigned int n)
{
    if (n == 0 || n == 1 || (n % 2 == 0))
    {
        return false;
    }
    else if(n == 2)
    {
        return true;
    }

    double num = n;
    double max = floor(sqrt(n));

    for (double i = 1; i <= max; i += 2)
    {
        if (i - 1 < 0.0001)
        {
            continue;
        }

        if (i / num < 0.0001)
        {
            return false;
        }
    }
    return true;
}