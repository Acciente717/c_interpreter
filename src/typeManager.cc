/* Copyright @2019 Zhiyao Ma */

#include "inc/typeManager.hh"
#include "inc/exceptions.hh"

namespace cint
{

typeManager& getTypeMgr()
{
    static typeManager typeMgr;
    return typeMgr;
}

fieldInfo typeInfo::getFieldByName(const std::string &queryName)
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


typeInfo typeManager::getTypeByNum(int _typeNum)
{
    try
    {
        return types.at(_typeNum);
    }
    catch (const std::out_of_range& oor)
    {
        throw unknownTypeNum(std::to_string(_typeNum));
    }
}


std::string typeManager::getTypenameByNum(int _typeNum)
{
    try
    {
        return types.at(_typeNum).getName();
    }
    catch (const std::out_of_range& oor)
    {
        throw unknownTypeNum(std::to_string(_typeNum));
    }
}


int typeManager::getTypeNumByName(const std::string &_typeName)
{
    try
    {
        return name2num.at(_typeName);
    }
    catch (const std::out_of_range& oor)
    {
        throw unknownTypeNum(_typeName);
    }
}

}  // namespace cint
