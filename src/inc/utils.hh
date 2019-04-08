/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_UTILS_HH_
#define SRC_INC_UTILS_HH_

#include <cstdlib>
#include <string>
#include <unordered_set>

#include "representations.hh"

namespace cint
{

/**
 *  Generate a random string with length `len`.
 *  If `sharpHead` is set to `true`, the first character will be '#',
 *  which is used to indicate a temporary variable name.
 */
std::string genRandomStr(const size_t len, const bool sharpHead = false);


/**
 *  Check if a given name exists in a hash set.
 */
inline bool isTempNameExist(const std::string &name,
                        const std::unordered_set<std::string> &set)
{
    auto it = set.find(name);
    if (it == set.end())
        return false;
    return true;
}

}  // namespace cint

#endif  // SRC_INT_UTILS_HH_
