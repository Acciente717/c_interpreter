/* Copyright @2019 Zhiyao Ma */
#include "utils.hh"

namespace cint
{

std::string genRandomStr(const size_t len, const bool sharpHead)
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    iferr (len == 0)
        return std::string();

    int i;
    std::string str(len, 0);
    if (sharpHead)
        str[0] = '#', i = 1;
    else
        i = 0;

    for (; i < len; ++i)
    {
        str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return str;
}

}  // namespace cint
