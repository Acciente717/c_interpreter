#ifndef __EXCEPTIONS_HH__
#define __EXCEPTIONS_HH__

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
    virtual const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

}

#endif
