#include "utils.h"

void print_thisis(long x)
{
    cputchar(116); cputchar(104); cputchar(105);  cputchar(115);
    cputchar(32); cputchar(105);  cputchar(115); cputchar(58);
    cputchar(32); cputint(x); cputchar(10);
}

void elif_nine_to_zero(long x)
{
    if (x == 0) { print_thisis(0); }
    else if (x == 1) { print_thisis(1); }
    else if (x == 2) { print_thisis(2); }
    else if (x == 3) { print_thisis(3); }
    else if (x == 4) { print_thisis(4); }
    else if (x == 5) { print_thisis(5); }
    else if (x == 6) { print_thisis(6); }
    else if (x == 7) { print_thisis(7); }
    else if (x == 8) { print_thisis(8); }
    else if (x == 9) { print_thisis(9); }
}

void main()
{
    long x = 0; long y = 1;

    if (x % 2) { cputint(0); cputchar(10); }
    if (y % 2) { cputint(1); cputchar(10); }

    if (x) { cputint(0); cputchar(10); }
    else { cputint(1); cputchar(10); }

    if (!x) { cputint(1); cputchar(10); }
    else { cputint(0); cputchar(10); }

    if (x)
    {
        if (x) { cputint(0); cputchar(10); }
        if (y) { cputint(0); cputchar(10); }
    }
    if (y)
    {
        if (x) { cputint(0); cputchar(10); }
        if (y) { cputint(1); cputchar(10); }
    }

    if (x)
    {
        if (x) { cputint(0); cputchar(10); }
        else { cputint(0); cputchar(10); }
    }
    else
    {
        if (x) { cputint(0); cputchar(10); }
        else { cputint(1); cputchar(10); }
    }

    if (!x) { cputint(1); cputchar(10); }
    if (!y) { cputint(0); cputchar(10); }

    if (x && y) { cputint(0); cputchar(10); }
    if (!x && y) { cputint(1); cputchar(10); }
    if (x && !y) { cputint(0); cputchar(10); }
    if (!x && !y) { cputint(0); cputchar(10); }

    if (x || y) { cputint(1); cputchar(10); }
    if (!x || y) { cputint(1); cputchar(10); }
    if (x || !y) { cputint(0); cputchar(10); }
    if (!x || !y) { cputint(1); cputchar(10); }

    if (x || y && x || y) { cputint(1); cputchar(10); }
    if (x || y && !x || y) { cputint(1); cputchar(10); }
    if (x || y && x || !y) { cputint(0); cputchar(10); }
    if (x || y && !x || !y) { cputint(1); cputchar(10); }
    if (!x || y && x || y) { cputint(1); cputchar(10); }
    if (!x || y && !x || y) { cputint(1); cputchar(10); }
    if (!x || y && x || !y) { cputint(1); cputchar(10); }
    if (!x || y && !x || !y) { cputint(1); cputchar(10); }
    if (x || !y && x || y) { cputint(1); cputchar(10); }
    if (x || !y && !x || y) { cputint(1); cputchar(10); }
    if (x || !y && x || !y) { cputint(0); cputchar(10); }
    if (x || !y && !x || !y) { cputint(0); cputchar(10); }
    if (!x || !y && x || y) { cputint(1); cputchar(10); }
    if (!x || !y && !x || y) { cputint(1); cputchar(10); }
    if (!x || !y && x || !y) { cputint(1); cputchar(10); }
    if (!x || !y && !x || !y) { cputint(1); cputchar(10); }

    if (x && y || x && y) { cputint(0); cputchar(10); }
    if (x && y || !x && y) { cputint(1); cputchar(10); }
    if (x && y || x && !y) { cputint(0); cputchar(10); }
    if (x && y || !x && !y) { cputint(0); cputchar(10); }
    if (!x && y || x && y) { cputint(1); cputchar(10); }
    if (!x && y || !x && y) { cputint(1); cputchar(10); }
    if (!x && y || x && !y) { cputint(1); cputchar(10); }
    if (!x && y || !x && !y) { cputint(1); cputchar(10); }
    if (x && !y || x && y) { cputint(0); cputchar(10); }
    if (x && !y || !x && y) { cputint(1); cputchar(10); }
    if (x && !y || x && !y) { cputint(0); cputchar(10); }
    if (x && !y || !x && !y) { cputint(0); cputchar(10); }
    if (!x && !y || x && y) { cputint(0); cputchar(10); }
    if (!x && !y || !x && y) { cputint(1); cputchar(10); }
    if (!x && !y || x && !y) { cputint(0); cputchar(10); }
    if (!x && !y || !x && !y) { cputint(0); cputchar(10); }

    long cnt = 9;
    while (cnt >= 0)
    {
        elif_nine_to_zero(cnt);
        cnt = cnt - 1;
    }
}
