/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_BUILTIN_FUNCTIONS_HH_
#define SRC_INC_BUILTIN_FUNCTIONS_HH_

#include <iostream>
#include <string>
#include <type_traits>

namespace cint
{

template <typename T>
inline void cputint(T val)
{
    static_assert(std::is_integral<T>::value, "cputint must be called "
                                              "with an integral argument!");
    std::cout << std::to_string(val);
}

template <typename T>
inline void cputfloat(T val)
{
    static_assert(std::is_floating_point<T>::value, "cputfloat must be called "
                                            "with an floating point argument!");
    std::cout << std::to_string(val);
}

template <typename T>
inline void cputchar(T val)
{
    static_assert(std::is_integral<T>::value, "cputchar must be called "
                                              "with an integral argument!");
    std::cout << static_cast<char>(val);
}

}  // namespace cint

#endif
