#include "utils.h"

long echo(long x)
{
    return x;
}

void main()
{
    long i = 0;
    while (i < 10000)
    {
        cputint(echo(i));
        cputchar(32);
        if (i % 16 == 15) { cputchar(10); }
        i = i + 1;
    }
}
