/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_BUILTIN_FUNCTIONS_HH_
#define SRC_INC_BUILTIN_FUNCTIONS_HH_

#include <iostream>
#include <string>
#include <type_traits>

template <typename T>
inline void cputint(T val)
{
    static_assert(std::is_integral<T>::value, "cputint must be called "
                                              "with an integral argument!");
    std::cout << std::to_string(val);
}

template <typename T>
inline void cputchar(T val)
{
    static_assert(std::is_integral<T>::value, "cputchar must be called "
                                              "with an integral argument!");
    std::cout << static_cast<char>(val);
}

#endif
