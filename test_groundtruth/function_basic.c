#include "utils.h"

long gcd(long x, long y)
{
    if (y > x)
    {
        long tmp;
        tmp = y;
        y = x;
        x = tmp;
    }
    if (y == 0)
    {
        return x;
    }
    return gcd(y, x % y);
}

void hello()
{
    cputchar(104); cputchar(101); cputchar(108); cputchar(108); cputchar(111);
    cputchar(32); cputchar(119); cputchar(111); cputchar(114); cputchar(108);
    cputchar(100); cputchar(33); cputchar(10);
}

void vain_modify_builtin(long val)
{
    val = 23333;
}

struct dummy
{
    double val;
};

void vain_modify_struct(struct dummy s)
{
    s.val = 2.333;
}

void true_modify_array(long ar[], long size)
{
    long i = 0;
    while (i < size)
    {
        ar[i] = i;
        i = i + 1;
    }
}

void early_func_return(long until)
{
    long i = 0;
    while (1)
    {
        cputint(i);
        if (i == until)
        {
            return;
        }
        i = i + 1;
    }
}

void main()
{
    hello();
    cputchar(10);

    long val;
    val = 111;
    vain_modify_builtin(val);
    cputint(val);
    cputchar(10);
    cputchar(10);

    struct dummy s;
    s.val = 1.111;
    vain_modify_struct(s);
    cputdouble(s.val);
    cputchar(10);
    cputchar(10);

    long ar[10];
    true_modify_array(ar, 10);
    long i = 0;
    while (i < 10)
    {
        cputint(ar[i]);
        i = i + 1;
    }
    cputchar(10);
    cputchar(10);

    early_func_return(9);
    cputchar(10);
    cputchar(10);

    cputint(gcd(21, 6));
    cputchar(10);
    cputint(gcd(2, 2));
    cputchar(10);
    cputint(gcd(88, 128));
    cputchar(10);
    cputint(gcd(496798142, 21937823));
    cputchar(10);
    cputchar(10);
}
