/* Copyright @2019 Zhiyao Ma */

#include <iostream>

#include "typeManager.hh"
#include "representations.hh"
#include "exceptions.hh"

namespace cint
{

const char *basicTypesName[] = {
    "long",
    "double",
    "void",
    "#array"
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

const fieldInfo *typeInfo::getFieldByName(const std::string &queryName) const
{
    try
    {
        return &fields.at(queryName);
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

long typeManager::getSizeByNum(long _typeNum)
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

inline static long roundup(long val, long mul)
{
    return (val + mul - 1) / mul * mul;
}

void typeManager::defineStructure(
    std::string &&structName,
    std::vector<fieldTextInfo> &&infos)
{
    long offset = 0;
    long maxSingleSize = 0;
    std::unordered_map<std::string, fieldInfo> fields;
    for (auto &info : infos)
    {
        auto typeNum = getTypeMgr().getTypeNumByName(info.typeName);
        auto typeSize = getTypeMgr().getSizeByNum(typeNum);
        offset = roundup(offset, typeSize);
        if (info.idxs.empty())
        {
            fieldInfo tmp{offset, typeSize, typeNum, info.varName,
                          {}, 0};
            fields.emplace(info.varName, std::move(tmp));
            offset += typeSize;
        }
        else
        {
            long dimNum = info.idxs.size();
            std::vector<long> dimSizes(dimNum);
            dimSizes[dimNum - 1] = typeSize;
            for (auto i = dimNum - 2; i >= 0; --i)
                dimSizes[i] = dimSizes[i + 1] * info.idxs[i + 1];
            auto fieldSize = dimSizes[0] * info.idxs[0];
            fieldInfo tmp{offset, dimSizes[0] * info.idxs[0], typeNum,
                          info.varName, std::move(dimSizes), info.idxs[0]};
            fields.emplace(info.varName, std::move(tmp));
            offset += fieldSize;
        }
        maxSingleSize = std::max(maxSingleSize, typeSize);
    }
    offset = roundup(offset, maxSingleSize);

    typeInfo tmpType{nextTypeNum, offset, structName,
                     std::move(fields)};
    types.emplace(nextTypeNum, std::move(tmpType));
    name2num.emplace(structName, nextTypeNum);
    ++nextTypeNum;
}

const fieldInfo *typeManager::getFieldInfo(long _typeNum,
                                           const std::string &_fieldName)
{
    auto iter = types.find(_typeNum);
    if (unlikely(iter == types.end()))
        throw unknownTypeNum(_typeNum);

    return iter->second.getFieldByName(_fieldName);
}

}  // namespace cint
