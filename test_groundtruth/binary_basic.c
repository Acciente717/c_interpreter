#include "utils.h"

void main()
{
    long ival;
    ival = 2;
    cputint(ival);
    cputchar(10);
    ival = -2;
    cputint(ival);
    cputchar(10);
    ival = -ival;
    cputint(ival);
    cputchar(10);
    ival = !ival;
    cputint(ival);
    cputchar(10);
    ival = !ival;
    cputint(ival);
    cputchar(10);

    double fval;
    fval = 2.0;
    cputdouble(fval);
    cputchar(10);
    fval = -2.0;
    cputdouble(fval);
    cputchar(10);
    fval = -fval;
    cputdouble(fval);
    cputchar(10);
    fval = !fval;
    cputdouble(fval);
    cputchar(10);
    fval = !fval;
    cputdouble(fval);
    cputchar(10);

    fval = ival = 2333;
    cputint(ival);
    cputchar(10);
    cputdouble(fval);
    cputchar(10);
}
