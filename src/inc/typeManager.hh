/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_TYPE_MANAGER_HH_
#define SRC_INC_TYPE_MANAGER_HH_

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

namespace cint
{

/*************************************************
*               Type Declarations                *
*************************************************/

enum basicTypesEnum
{
    CLONG,
    CDOUBLE,
    CVOID,
    CARRAY,
    basicTypesNum
};

extern const char *basicTypesName[];

extern const long basicTypesSize[];

struct fieldInfo
{
    long offset;
    long fieldSize;
    long baseTypeNum;
    std::string name;
    std::vector<long> dimSizes;
    long topLayerShape;
};

struct fieldTextInfo
{
    std::string typeName;
    std::string varName;
    std::vector<long> idxs;
};

class typeInfo
{
    long typeNum;                     // number of the type
    long typeSize;                    // size of the type
    bool basic;                       // whether the size is one of the basics
    std::string typeName;             // name of the type
    std::unordered_map<std::string, fieldInfo>
        fields;                       // fields contained in the type
 public:
    inline explicit typeInfo(decltype(typeNum) _typeNum,
                             decltype(typeSize) _size,
                             const decltype(typeName) &_typeName,
                             const decltype(fields) &_fields) noexcept;
    inline explicit typeInfo(decltype(typeNum) _typeNum,
                             decltype(typeSize) _size,
                             decltype(typeName) &&_typeName,
                             decltype(fields) &&_fields) noexcept;
    inline bool isBasic() const noexcept;
    inline std::string getName() const noexcept;
    inline long getSize() const noexcept;
    inline bool operator==(const typeInfo &rhs) const noexcept;
    const fieldInfo *getFieldByName(const std::string &queryName) const;
};

class typeManager
{
    std::unordered_map<long, typeInfo> types;
    std::unordered_map<std::string, long> name2num;
    long nextTypeNum;

    inline void initBuiltinTypes();
 public:
    inline typeManager();
    static long chooseSuperType(long lhs, long rhs)
    {
        return lhs > rhs ? lhs : rhs;
    }

    typeInfo getTypeByNum(long _typeNum);
    std::string getTypenameByNum(long _typeNum);
    long getTypeNumByName(const std::string &_typeName);
    long getSizeByNum(long _typeNum);
    void defineStructure(
        std::string &&structName,
        std::vector<fieldTextInfo> &&infos);
    const fieldInfo *getFieldInfo(long _typeNum,
                                  const std::string &_fieldName);
};

typeManager& getTypeMgr();

/*************************************************
*              Type Implementations              *
*************************************************/

typeInfo::typeInfo(decltype(typeNum) _typeNum,
                   decltype(typeSize) _size,
                   const decltype(typeName) &_typeName,
                   const decltype(fields) &_fields) noexcept
    : typeNum(_typeNum), typeSize(_size), typeName(_typeName),
      fields(_fields)
{
    basic = _typeNum < basicTypesNum;
}

typeInfo::typeInfo(decltype(typeNum) _typeNum,
                   decltype(typeSize) _size,
                   decltype(typeName) &&_typeName,
                   decltype(fields) &&_fields) noexcept
    : typeNum(_typeNum), typeSize(_size), typeName(_typeName),
      fields(_fields)
{
    basic = _typeNum < basicTypesNum;
}


inline bool typeInfo::operator==(const typeInfo &rhs) const noexcept
{
    if (typeNum != rhs.typeNum)
        return false;
    return true;
}

inline std::string typeInfo::getName() const noexcept
{
    return typeName;
}

inline long typeInfo::getSize() const noexcept
{
    return typeSize;
}

inline bool typeInfo::isBasic() const noexcept
{
    return basic;
}

inline typeManager::typeManager()
{
    initBuiltinTypes();
}

inline void typeManager::initBuiltinTypes()
{
    for (long i = 0; i < basicTypesNum; ++i)
    {
        types.emplace(i, typeInfo(i, basicTypesSize[i],
                            basicTypesName[i],
                            std::unordered_map<std::string, fieldInfo>()));
        name2num.emplace(basicTypesName[i], i);
    }
    nextTypeNum = basicTypesNum + 1;
}

}  // namespace cint

#endif  // SRC_INC_TYPE_MANAGER_HH_
