#include "utils.h"

struct single_long
{
    long val;
};

struct single_double
{
    double val;
};

struct array_long
{
    long ar[10][10];
};

void main()
{
    long cnt1;
    long cnt2;

    struct single_long s1;
    s1.val = 2333;
    cputint(s1.val);
    cputchar(10);

    struct single_double s2;
    s2.val = 2.333;
    cputdouble(s2.val);
    cputchar(10);

    cputint(s1.val);
    cputchar(10);

    struct array_long s3;
    cnt1 = 0;
    while (cnt1 < 10)
    {
        cnt2 = 0;
        while (cnt2 < 10)
        {
            s3.ar[cnt1][cnt2] = cnt2;
            cnt2 = cnt2 + 1;
        }
        cnt1 = cnt1 + 1;
    }
    cnt1 = 0;
    while (cnt1 < 10)
    {
        cnt2 = 0;
        while (cnt2 < 10)
        {
            cputint(s3.ar[cnt1][cnt2]);
            cnt2 = cnt2 + 1;
        }
        cputchar(10);
        cnt1 = cnt1 + 1;
    }

    struct single_long sar[10];
    cnt1 = 0;
    while (cnt1 < 10)
    {
        sar[cnt1].val = 9 - cnt1;
        cnt1 = cnt1 + 1;
    }
    cnt1 = 0;
    while (cnt1 < 10)
    {
        cputint(sar[cnt1].val);
        cnt1 = cnt1 + 1;
    }
    cputchar(10);

    struct array_long aar[2];
    long cnt3;
    cnt1 = 0;
    while (cnt1 < 2)
    {
        cnt2 = 0;
        while (cnt2 < 10)
        {
            cnt3 = 0;
            while (cnt3 < 10)
            {
                cnt3 = cnt3 + 1;
                aar[cnt1].ar[cnt2][cnt3] = cnt2;
            }
            cnt2 = cnt2 + 1;
        }
        cnt1 = cnt1 + 1;
    }
    cnt1 = 0;
    while (cnt1 < 2)
    {
        cnt2 = 0;
        while (cnt2 < 10)
        {
            cnt3 = 0;
            while (cnt3 < 10)
            {
                cnt3 = cnt3 + 1;
                cputint(aar[cnt1].ar[cnt2][cnt3]);
            }
            cnt2 = cnt2 + 1;
            cputchar(10);
        }
        cnt1 = cnt1 + 1;
    }
}
