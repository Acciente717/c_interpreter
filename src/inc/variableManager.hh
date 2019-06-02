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
};

using variableDictionary = std::unordered_map<std::string, VariableInfo>;

class VariableManager
{
    std::vector<variableDictionary> varStack;

    inline VariableInfo *searchVariable(
        std::string varName
    );
 public:
    inline VariableManager();
    ~VariableManager() noexcept;

    inline void newScope();
    void popScope();
    void declareVariable(const std::string &typeName,
                         const std::string &varName);
    void initializeVariable(const std::string &typeName,
                            const std::string &varName,
                            const void *initData);
    void assignVariable(const std::string &varName, const void *data);
    void *getVariableData(const std::string &varName);
    int getVariableTypeNum(const std::string &varName);
};

VariableManager& getVarMgr();

/*************************************************
*              Type Implementations              *
*************************************************/

inline VariableManager::VariableManager()
{
    varStack.emplace_back();
}

inline VariableInfo* VariableManager::searchVariable(
    std::string varName)
{
    auto iter = varStack.back().find(varName);
    if (iter == varStack.back().end())
        return nullptr;
    return &(iter->second);
}

inline void VariableManager::newScope()
{
    varStack.emplace_back();
}

}  // namespace cint

#endif  // SRC_INC_VARIABLE_MANAGER_HH_
