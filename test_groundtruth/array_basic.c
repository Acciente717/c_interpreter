#include "utils.h"

void fill_ten(long ar[10])
{
    long i = 9;
    while (i >= 0)
    {
        ar[9 - i] = i;
        i = i - 1;
    }
}

void fill_any(long ar[], long len)
{
    long i = 0;
    while (i < len)
    {
        ar[i] = i;
        i = i + 1;
    }
}

void reinterpret_fill(long ar[4][5])
{
    long i = 0;
    long j;
    while (i < 4)
    {
        j = 0;
        while (j < 5)
        {
            ar[i][j] = j;
            j = j + 1;
        }
        i = i + 1;
    }
}

void main()
{
    long ar1[1];
    ar1[0] = 2333;
    cputint(ar1[0]);
    cputchar(10);

    long ar2[10];
    long cnt = 9;
    while (cnt >= 0)
    {
        ar2[cnt] = cnt;
        cnt = cnt - 1;
    }
    cnt = 9;
    while (cnt >= 0)
    {
        cputint(ar2[cnt]);
        cnt = cnt - 1;
    }
    cputchar(10);

    long ar3[1][10];
    cnt = 9;
    while (cnt >= 0)
    {
        ar3[0][cnt] = cnt;
        cnt = cnt - 1;
    }
    cnt = 9;
    while (cnt >= 0)
    {
        cputint(ar3[0][cnt]);
        cnt = cnt - 1;
    }
    cputchar(10);

    long cnt2 = 9;
    long ar4[10][10];
    while (cnt2 >= 0)
    {
        cnt = 9;
        while (cnt >= 0) { ar4[cnt2][cnt] = cnt2; cnt = cnt - 1; }
        cnt2 = cnt2 - 1;
    }
    cnt2 = 9;
    while (cnt2 >= 0)
    {
        cnt = 9;
        while (cnt >= 0) { cputint(ar4[cnt2][cnt]); cnt = cnt - 1; }
        cputchar(10);
        cnt2 = cnt2 - 1;
    }

    long ar5[10];
    fill_ten(ar5);
    cnt = 0;
    while (cnt < 10)
    {
        cputint(ar5[cnt]);
        cnt = cnt + 1;
    }
    cputchar(10);
    fill_any(ar5, 10);
    cnt = 0;
    while (cnt < 10)
    {
        cputint(ar5[cnt]);
        cnt = cnt + 1;
    }
    cputchar(10);

    long ar6[2][10];
    reinterpret_fill(ar6);
    cnt2 = 0;
    while (cnt2 < 2)
    {
        cnt = 0;
        while (cnt < 10)
        {
            cputint(ar6[cnt2][cnt]);
            cnt = cnt + 1;
        }
        cputchar(10);
        cnt2 = cnt2 + 1;
    }
}
