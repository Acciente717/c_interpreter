/* Copyright Zhiyao Ma 2019 */
#ifndef TEST_GROUNDTRUTH_UTILS_H_
#define TEST_GROUNDTRUTH_UTILS_H_

#include <stdio.h>

inline void cputint(long x)
{
    printf("%ld", x);
}

inline void cputdouble(double x)
{
    printf("%f", x);
}

inline void cputchar(long x)
{
    printf("%c", (char) x);
}

#endif  // TEST_GROUNDTRUTH_UTILS_H_
