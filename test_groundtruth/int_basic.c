#include "utils.h"

void main()
{
    long val1;
    long val2 = 2;
    cputint(val2);
    cputchar(10);
    long val3 = val2;
    cputint(val3);
    cputchar(10);
    val1 = 3;
    cputint(val1);
    cputchar(10);
    val2 = val1;
    cputint(val2);
    cputchar(10);
    val2 = val2;
    cputint(val2);
    cputchar(10);
}
