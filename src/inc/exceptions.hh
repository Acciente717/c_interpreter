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

class unknownFieldName : public std::exception
{
    std::string msg;
 public:
    explicit unknownFieldName(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

class unknownTypeNum : public std::exception
{
    std::string msg;
 public:
    explicit unknownTypeNum(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

class unknownTypeName : public std::exception
{
    std::string msg;
 public:
    explicit unknownTypeName(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

class duplicateVariableName : public std::exception
{
    std::string msg;
 public:
    explicit duplicateVariableName(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

class unknownVariableName : public std::exception
{
    std::string msg;
 public:
    explicit unknownVariableName(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

class unsupportedArithmic : public std::exception
{
    std::string msg;
 public:
    explicit unsupportedArithmic(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

class unknownSwitchCase : public std::exception
{
    std::string msg;
 public:
    explicit unknownSwitchCase(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

class notImplemented : public std::exception
{
    std::string msg;
 public:
    explicit notImplemented(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

class badIntermediateCommand : public std::exception
{
    std::string msg;
 public:
    explicit badIntermediateCommand(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

class redefiningFunction : public std::exception
{
    std::string msg;
 public:
    explicit redefiningFunction(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

class unknownFunction : public std::exception
{
    std::string msg;
 public:
    explicit unknownFunction(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

class funcArgumentError : public std::exception
{
    std::string msg;
 public:
    explicit funcArgumentError(const std::string &s)
        : msg(s) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

}  // namespace cint

#endif  // SRC_INC_EXCEPTIONS_HH
