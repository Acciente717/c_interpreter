/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_EXCEPTIONS_HH
#define SRC_INC_EXCEPTIONS_HH

#include <exception>
#include <string>

namespace cint
{

class cmdCreationError : public std::exception
{
    std::string msg;
 public:
    explicit cmdCreationError(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

class yaccInfoCreationError : public std::exception
{
    std::string msg;
 public:
    explicit yaccInfoCreationError(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

}  // namespace cint

#endif  // SRC_INC_EXCEPTIONS_HH
