#include "utils.h"

void main()
{
    double val1;
    double val2 = 0.5;
    cputdouble(val2);
    cputchar(10);
    double val3 = val2;
    cputdouble(val3);
    cputchar(10);
    val1 = 2.5;
    cputdouble(val1);
    cputchar(10);
    val2 = val1;
    cputdouble(val2);
    cputchar(10);
    val2 = val2;
    cputdouble(val2);
    cputchar(10);
}
