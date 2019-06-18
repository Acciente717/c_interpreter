#include "utils.h"

void output(long x[], long len)
{
    long i = 0;
    while (i < len)
    {
        cputint(x[i]);
        cputchar(32);
        i = i + 1;
    }
}

void fill(long x[], long len, long val)
{
    long i = 0;
    while (i < len)
    {
        x[i] = val;
        i = i + 1;
    }
}

void main()
{
    long xx[10];
    long i = 0;
    while (i < 2000)
    {
        fill(xx, 10, i);
        output(xx, 10);
        cputchar(10);
        i = i + 1;
    }
}
