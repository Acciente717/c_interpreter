/* Copyright @2019 Zhiyao Ma */

#include <iostream>
#include "yaccInfoStructure.hh"

namespace cint
{

void yaccInfo::releaseMemory()
{
    switch (type)
    {
    case infoType::empty:
    case infoType::operatorType:
        data = nullptr;
        break;
    case infoType::typeName:
    case infoType::varName:
    case infoType::literal:
        delete reinterpret_cast<std::string *>(data);
        break;
    }
}

yaccInfo::~yaccInfo()
{
    releaseMemory();
}

yaccInfo& yaccInfo::operator=(const yaccInfo &rhs)
{
    releaseMemory();
    type = rhs.type;
    switch (type)
    {
    case infoType::empty:
    case infoType::operatorType:
        data = rhs.data;
        break;
    case infoType::typeName:
    case infoType::varName:
    case infoType::literal:
        data = new std::string(*reinterpret_cast<std::string *>(rhs.data));
        break;
    }
    return *this;
}

yaccInfo& yaccInfo::operator=(yaccInfo &&rhs)
{
    releaseMemory();
    type = rhs.type;
    switch (type)
    {
    case infoType::empty:
    case infoType::operatorType:
        data = rhs.data;
        break;
    case infoType::typeName:
    case infoType::varName:
    case infoType::literal:
        data = new std::string(std::move(*reinterpret_cast<std::string *>
                                            (rhs.data)));
        break;
    }
    return *this;
}

}  // namespace cint
