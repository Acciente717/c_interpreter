#include "utils.h"

void main()
{
    long ival1 = 1.5;
    cputint(ival1);
    cputchar(10);
    ival1 = 0.5;
    cputint(ival1);
    cputchar(10);
    double fval = 2.5;
    long ival2 = fval;
    cputint(ival2);
    cputchar(10);
    fval = 3.5;
    ival2 = fval;
    cputint(ival2);
    cputchar(10);
}
