#include "utils.h"

void print_thisis(long x)
{
    cputchar(116); cputchar(104); cputchar(105);  cputchar(115);
    cputchar(32); cputchar(105);  cputchar(115); cputchar(58);
    cputchar(32); cputint(x); cputchar(10);
}

void main()
{
    long cnt = 5;

    while (cnt >= 0) { print_thisis(cnt); cnt = cnt - 1; }
    cputchar(10);
    
    long cnt2 = 2;
    while (cnt2 >= 0)
    {
        cnt = 3;
        while (cnt >= 0) { print_thisis(cnt); cnt = cnt - 1; }
        cnt2 = cnt2 - 1;
    }
    cputchar(10);

    cnt = 5;
    while (cnt >= 0)
    {
        print_thisis(cnt);
        if (cnt == 2) { break; }
        cnt = cnt - 1;
    }
    cputchar(10);

    cnt2 = 2;
    while (cnt2 >= 0)
    {
        cnt = 5;
        while (cnt >= 0)
        {
            print_thisis(cnt);
            if (cnt == 2) { break; }
            cnt = cnt - 1;
        }
        cnt2 = cnt2 - 1;
    }
    cputchar(10);

    cnt = 5;
    while (cnt >= 0)
    {
        if (cnt == 2)
        {
            cnt = cnt - 1;
            continue;
        }
        print_thisis(cnt);
        cnt = cnt - 1;
    }
    cputchar(10);

    cnt2 = 2;
    while (cnt2 >= 0)
    {
        cnt = 5;
        while (cnt >= 0)
        {
            if (cnt == 2)
            {
                cnt = cnt - 1;
                continue;
            }
            print_thisis(cnt);
            cnt = cnt - 1;
        }
        cnt2 = cnt2 - 1;
    }
}
