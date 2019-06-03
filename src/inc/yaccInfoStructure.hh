/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_YACCINFOSTRUCTURE_HH_
#define SRC_INC_YACCINFOSTRUCTURE_HH_

#include <string>

#include "exceptions.hh"
#include "representations.hh"

namespace cint
{

/**
 *  Storage structore for information passing on the grammar tree generated
 *  by YACC.
 */
struct yaccInfo
{
 public:
    /**
     *  Primary type of the information.
     */
    enum class infoType
    {
        empty,
        typeName,
        varName,
        literal,
        operatorType
    };

    /**
     *  Sub-type of the information when the primary type is `operatorType`.
     */
    enum class infoOprType
    {
        add,
        sub,
        mul,
        div,
        mod
    };

 public:
    infoType type;
    void *data;

    void releaseMemory();

 public:
    inline yaccInfo();
    inline explicit yaccInfo(infoType type, infoOprType oprType);
    inline explicit yaccInfo(infoType type, const std::string &name);
    inline explicit yaccInfo(infoType type, std::string &&name);
    yaccInfo& operator=(const yaccInfo &rhs);
    yaccInfo& operator=(yaccInfo &&rhs);
    ~yaccInfo();
};

yaccInfo::yaccInfo() : type(infoType::empty), data(nullptr) {}

yaccInfo::yaccInfo(infoType _type, infoOprType _oprType)
{
    if (_type != infoType::operatorType)
        throw yaccInfoCreationError("Error: argument mismatch"
                                    " while creating yaccInfo of"
                                    " operatorType!");
    type = _type;
    data = reinterpret_cast<void *>(_oprType);
}

yaccInfo::yaccInfo(infoType _type, const std::string &name)
{
    if (_type != infoType::varName && _type != infoType::typeName
        && _type != infoType::literal)
        throw yaccInfoCreationError("Error: argument mismatch"
                                    " while creating yaccInfo of"
                                    " varName or typeName or"
                                    " literal!");
    type = _type;
    data = new std::string(name);
}

yaccInfo::yaccInfo(infoType _type, std::string &&name)
{
    if (_type != infoType::varName && _type != infoType::typeName
        && _type != infoType::literal)
        throw yaccInfoCreationError("Error: argument mismatch"
                                    " while creating yaccInfo of"
                                    " varName or typeName or"
                                    " literal!");
    type = _type;
    data = new std::string(std::move(name));
}

}  // namespace cint

#endif  // SRC_INC_YACCINFOSTRUCTURE_HH_
