#include "utils.h"

void main()
{
    long ival1;
    long ival2;
    long ival3;
    ival1 = 2333; ival2 = 1111;

    ival3 = 2333 + 1111;
    cputint(ival3);
    cputchar(10);
    ival3 = 2333 + ival2;
    cputint(ival3);
    cputchar(10);
    ival3 = ival1 + 1111;
    cputint(ival3);
    cputchar(10);
    ival3 = ival1 + ival2;
    cputint(ival3);
    cputchar(10);
    cputchar(10);

    ival3 = 2333 - 1111;
    cputint(ival3);
    cputchar(10);
    ival3 = 2333 - ival2;
    cputint(ival3);
    cputchar(10);
    ival3 = ival1 - 1111;
    cputint(ival3);
    cputchar(10);
    ival3 = ival1 - ival2;
    cputint(ival3);
    cputchar(10);
    cputchar(10);

    ival3 = 2333 * 1111;
    cputint(ival3);
    cputchar(10);
    ival3 = 2333 * ival2;
    cputint(ival3);
    cputchar(10);
    ival3 = ival1 * 1111;
    cputint(ival3);
    cputchar(10);
    ival3 = ival1 * ival2;
    cputint(ival3);
    cputchar(10);
    cputchar(10);

    ival3 = 2333 / 1111;
    cputint(ival3);
    cputchar(10);
    ival3 = 2333 / ival2;
    cputint(ival3);
    cputchar(10);
    ival3 = ival1 / 1111;
    cputint(ival3);
    cputchar(10);
    ival3 = ival1 / ival2;
    cputint(ival3);
    cputchar(10);
    cputchar(10);

    ival3 = 2333 % 1111;
    cputint(ival3);
    cputchar(10);
    ival3 = 2333 % ival2;
    cputint(ival3);
    cputchar(10);
    ival3 = ival1 % 1111;
    cputint(ival3);
    cputchar(10);
    ival3 = ival1 % ival2;
    cputint(ival3);
    cputchar(10);
    cputchar(10);

    double fval1;
    double fval2;
    double fval3;
    fval1 = 2333.0; fval2 = 1111.0;

    fval3 = 2333.0 + 1111.0;
    cputdouble(fval3);
    cputchar(10);
    fval3 = 2333.0 + fval2;
    cputdouble(fval3);
    cputchar(10);
    fval3 = fval1 + 1111.0;
    cputdouble(fval3);
    cputchar(10);
    fval3 = fval1 + fval2;
    cputdouble(fval3);
    cputchar(10);
    cputchar(10);

    fval3 = 2333.0 - 1111.0;
    cputdouble(fval3);
    cputchar(10);
    fval3 = 2333.0 - fval2;
    cputdouble(fval3);
    cputchar(10);
    fval3 = fval1 - 1111.0;
    cputdouble(fval3);
    cputchar(10);
    fval3 = fval1 - fval2;
    cputdouble(fval3);
    cputchar(10);
    cputchar(10);

    fval3 = 2333.0 * 1111.0;
    cputdouble(fval3);
    cputchar(10);
    fval3 = 2333.0 * fval2;
    cputdouble(fval3);
    cputchar(10);
    fval3 = fval1 * 1111.0;
    cputdouble(fval3);
    cputchar(10);
    fval3 = fval1 * fval2;
    cputdouble(fval3);
    cputchar(10);
    cputchar(10);

    fval3 = 2333.0 / 1111.0;
    cputdouble(fval3);
    cputchar(10);
    fval3 = 2333.0 / fval2;
    cputdouble(fval3);
    cputchar(10);
    fval3 = fval1 / 1111.0;
    cputdouble(fval3);
    cputchar(10);
    fval3 = fval1 / fval2;
    cputdouble(fval3);
    cputchar(10);
}
