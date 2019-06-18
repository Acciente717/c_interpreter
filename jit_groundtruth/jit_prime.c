#include "utils.h"

double sqrt(double x)
{
    double nxt = 1;
    double pre = 1;
    while (1)
    {
        nxt = (x + pre * pre) / (2 * pre);
        if (nxt - pre < 0.00001 && pre - nxt < 0.00001)
        {
            break;
        }
        pre = nxt;
    }
    return nxt;
}

long judge(long x)
{
    if (x % 2 == 0) { return 0; }
    if (x % 3 == 0) { return 0; }
    if (x % 5 == 0) { return 0; }
    if (x % 7 == 0) { return 0; }
    long limit = sqrt(x) + 0.5;
    long i = 11;
    while (i <= limit)
    {
        if (x % i == 0) { return 0; }
        i = i + 2;
        if (x % i == 0) { return 0; }
        i = i + 4;
        if (x % i == 0) { return 0; }
        i = i + 2;
        if (x % i == 0) { return 0; }
        i = i + 2;
    }
    return 1;
}


long main()
{
    long limit = 500000;
    long test = 11;
    long have = 0;

    while (have < limit)
    {
        if (judge(test))
        {
            cputint(test);
            have = have + 1;
            cputchar(10);
        }
        test = test + 2;
    }
}
