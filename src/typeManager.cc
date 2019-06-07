/* Copyright @2019 Zhiyao Ma */

#include <iostream>

#include "typeManager.hh"
#include "exceptions.hh"

namespace cint
{

const char *basicTypesName[] = {
    "long",
    "double",
    "void",
    ""
};

const long basicTypesSize[] = {
    8,  // long
    8,  // double
    1,  // void
    1   // array
};

typeManager& getTypeMgr()
{
    static typeManager typeMgr;
    return typeMgr;
}

fieldInfo typeInfo::getFieldByName(const std::string &queryName) const
{
    try
    {
        return fields.at(queryName);
    }
    catch (const std::out_of_range& oor)
    {
        throw unknownFieldName(queryName);
    }
}


typeInfo typeManager::getTypeByNum(long _typeNum)
{
    try
    {
        return types.at(_typeNum);
    }
    catch (const std::out_of_range& oor)
    {
        throw unknownTypeNum(_typeNum);
    }
}


std::string typeManager::getTypenameByNum(long _typeNum)
{
    try
    {
        return types.at(_typeNum).getName();
    }
    catch (const std::out_of_range& oor)
    {
        throw unknownTypeName(std::to_string(_typeNum));
    }
}


long typeManager::getTypeNumByName(const std::string &_typeName)
{
    try
    {
        return name2num.at(_typeName);
    }
    catch (const std::out_of_range& oor)
    {
        throw unknownTypeName(_typeName);
    }
}

unsigned typeManager::getSizeByNum(long _typeNum)
{
    try
    {
        return types.at(_typeNum).getSize();
    }
    catch (const std::out_of_range& oor)
    {
        throw unknownTypeNum(_typeNum);
    }
}

}  // namespace cint
