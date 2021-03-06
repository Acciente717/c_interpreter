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

struct inner1
{
    long val;
};

struct inner2
{
    double val;
};

struct outer
{
    struct inner1 i1;
    struct inner2 i2;
};

struct array_long build()
{
    struct array_long ar;
    long i;
    long j;

    i = 0;
    while (i < 10)
    {
        j = 0;
        while (j < 10)
        {
            ar.ar[i][j] = j;
            j = j + 1;
        }
        i = i + 1;
    }
    return ar;
}

void print(struct single_double s)
{
    cputdouble(s.val);
    cputchar(10);
}

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

    print(s2);

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
                aar[cnt1].ar[cnt2][cnt3] = cnt2;
                cnt3 = cnt3 + 1;
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
                cputint(aar[cnt1].ar[cnt2][cnt3]);
                cnt3 = cnt3 + 1;
            }
            cnt2 = cnt2 + 1;
            cputchar(10);
        }
        cnt1 = cnt1 + 1;
    }

    struct array_long built_ar;
    built_ar = build();

    cnt1 = 0;
    while (cnt1 < 10)
    {
        cnt2 = 0;
        while (cnt2 < 10)
        {
            cputint(built_ar.ar[cnt1][cnt2]);
            cnt2 = cnt2 + 1;
        }
        cputchar(10);
        cnt1 = cnt1 + 1;
    }

    struct outer s4;
    s4.i1.val = 2333;
    s4.i2.val = 2.333;
    cputint(s4.i1.val);
    cputchar(10);
    cputdouble(s4.i2.val);
    cputchar(10);
}
