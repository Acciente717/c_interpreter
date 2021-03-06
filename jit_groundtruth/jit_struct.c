#include "utils.h"

struct struc
{
    long ival;
    double fval;
};

void modifyLong(struct struc ptr[], long val)
{
    ptr[0].ival = val;
}

void modifyDouble(struct struc sptr[], double dptr[])
{
    sptr[0].fval = dptr[0];
}

void main()
{
    long i = 0;
    struct struc s[1];
    double d[1];
    while (i < 50000)
    {
        modifyLong(s, i);
        d[0] = i;
        modifyDouble(s, d);
        cputint(s[0].ival);
        cputchar(32);
        cputdouble(s[0].fval);
        cputchar(10);
        i = i + 1;
    }
}
