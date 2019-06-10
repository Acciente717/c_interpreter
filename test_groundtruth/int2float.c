#include "utils.h"

void main()
{
    double fval1 = 1;
    cputdouble(fval1);
    cputchar(10);
    fval1 = 2;
    cputdouble(fval1);
    cputchar(10);
    long ival = 3;
    double fval2 = ival;
    cputdouble(fval2);
    cputchar(10);
    ival = 4;
    fval2 = ival;
    cputdouble(fval2);
    cputchar(10);
}
