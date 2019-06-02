/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_TYPE_MANAGER_HH_
#define SRC_INC_TYPE_MANAGER_HH_

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
    CINT32,
    CFLOAT
};

const char *basicTypesName[] = {
    "int",
    "float"
};

const int basicTypesSize[] = {
    4,  // int
    4   // float
};

static_assert(
    sizeof(basicTypesSize) / sizeof(int)
    == sizeof(basicTypesName) / sizeof(*basicTypesName),
    "inconsistent basic type number");
constexpr int basicTypesNum = sizeof(basicTypesSize) / sizeof(int);

struct fieldInfo
{
    unsigned offset;
    unsigned len;
    int typeNum;
    std::string name;
};

class typeInfo
{
    int typeNum;                      // number of the type
    unsigned typeSize;                // size of the type
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
    inline bool isBasic() noexcept;
    inline std::string getName() noexcept;
    inline unsigned getSize() noexcept;
    fieldInfo getFieldByName(const std::string &queryName);
};

class typeManager
{
    std::unordered_map<int, typeInfo> types;
    std::unordered_map<std::string, int> name2num;

    inline void initBuiltinTypes();
 public:
    static int chooseSuperType(int lhs, int rhs)
    {
        return lhs > rhs ? lhs : rhs;
    }

    typeInfo getTypeByNum(int _typeNum);
    std::string getTypenameByNum(int _typeNum);
    int getTypeNumByName(const std::string &_typeName);
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

inline std::string typeInfo::getName() noexcept
{
    return typeName;
}

inline unsigned typeInfo::getSize() noexcept
{
    return typeSize;
}

inline bool typeInfo::isBasic() noexcept
{
    return basic;
}

inline void typeManager::initBuiltinTypes()
{
    for (int i = 0; i < basicTypesNum; ++i)
    {
        types.emplace(i, typeInfo(i, basicTypesSize[i],
                            basicTypesName[i],
                            std::unordered_map<std::string, fieldInfo>()));
        name2num.emplace(basicTypesName[i], i);
    }
    assert(types.size() == basicTypesNum);
    assert(name2num.size() == basicTypesNum);
}

}  // namespace cint

#endif  // SRC_INC_TYPE_MANAGER_HH_
