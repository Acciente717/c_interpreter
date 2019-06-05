/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_VARIABLE_MANAGER_HH_
#define SRC_INC_VARIABLE_MANAGER_HH_

#include <vector>
#include <unordered_map>
#include <string>

#include "typeManager.hh"

namespace cint
{

/*************************************************
*               Type Declarations                *
*************************************************/

struct VariableInfo
{
    int typeNum;
    unsigned typeSize;
    void *data;

    VariableInfo() = delete;
    explicit VariableInfo(decltype(typeNum) _typeNum,
                          decltype(typeSize) _typeSize,
                          decltype(data) _data) noexcept
    {
        typeNum = _typeNum;
        typeSize = _typeSize;
        data = _data;
    }
    VariableInfo(const VariableInfo &other) = delete;
    VariableInfo(VariableInfo &&other) noexcept
    {
        typeNum = other.typeNum;
        typeSize = other.typeSize;
        data = other.data;
        other.data = nullptr;
    }
    VariableInfo& operator=(const VariableInfo &other) = delete;
    VariableInfo& operator=(VariableInfo &&other) noexcept
    {
        delete[] reinterpret_cast<uint8_t*>(data);
        typeNum = other.typeNum;
        typeSize = other.typeSize;
        data = other.data;
        other.data = nullptr;
        return *this;
    }
    ~VariableInfo() { delete[] reinterpret_cast<uint8_t*>(data); }
};

using variableDictionary = std::unordered_map<std::string, VariableInfo>;

class VariableManager
{
    std::vector<variableDictionary> varStack;
    std::vector<uint8_t> funcIndicatorStack;
    variableDictionary globals;
    VariableInfo returnValue;

    inline VariableInfo *searchVariableCurrentScope(
        const std::string &varName
    );

    VariableInfo *searchVariableRecursive(
        const std::string &varName
    );
 public:
    inline VariableManager();
    ~VariableManager() noexcept;

    inline void newScope(bool isFunction = false);
    void popScope();
    void declareVariable(const std::string &typeName,
                         const std::string &varName);
    void initializeVariable(const std::string &typeName,
                            const std::string &varName,
                            const void *initData);
    void assignVariable(const std::string &varName, const void *data);
    void *getVariableData(const std::string &varName);
    int getVariableTypeNum(const std::string &varName);
    void updateReturnValue(const std::string &typeName,
                           const void *updateData);
    void *getReturnValueData();
    int getReturnValueTypeNum();
    void setReturnValueToVoid();
};

VariableManager& getVarMgr();

/*************************************************
*              Type Implementations              *
*************************************************/

inline VariableManager::VariableManager()
    : returnValue(CVOID, 1, new uint8_t[1])
{
    varStack.emplace_back();
    funcIndicatorStack.push_back(0);
}

inline VariableInfo* VariableManager::searchVariableCurrentScope(
    const std::string &varName)
{
    auto iter = varStack.back().find(varName);
    if (iter == varStack.back().end())
        return nullptr;
    return &(iter->second);
}

inline void VariableManager::newScope(bool isFunction)
{
    varStack.emplace_back();
    funcIndicatorStack.push_back(static_cast<uint8_t>(isFunction));
}

}  // namespace cint

#endif  // SRC_INC_VARIABLE_MANAGER_HH_
