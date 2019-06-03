/* Copyright @2019 Zhiyao Ma */
#include <cstdint>
#include <memory>
#include <cstring>

#include "variableManager.hh"
#include "exceptions.hh"

namespace cint
{

VariableManager& getVarMgr()
{
    static VariableManager mgr;
    return mgr;
}


VariableManager::~VariableManager() noexcept
{
    while (varStack.size())
        popScope();
}

VariableInfo *VariableManager::searchVariableRecursive(
    const std::string &varName
)
{
    for (int i = varStack.size() - 1; i >= 0; --i)
    {
        auto iter = varStack[i].find(varName);
        if (iter != varStack[i].end())
            return &(iter->second);
    }
    return nullptr;
}

void VariableManager::declareVariable(const std::string &typeName,
                                      const std::string &varName)
{
    // duplicate variable declaration
    auto pVar = searchVariableCurrentScope(varName);
    if (pVar)
        throw duplicateVariableName(varName);

    auto typeNum = getTypeMgr().getTypeNumByName(typeName);
    auto type = getTypeMgr().getTypeByNum(typeNum);

    // insert new variable
    std::unique_ptr<uint8_t[]> data(new uint8_t[type.getSize()]);
    varStack.back().emplace(varName,
                            VariableInfo{typeNum, type.getSize(), data.get()});
    data.release();
}


void VariableManager::initializeVariable(const std::string &typeName,
                                         const std::string &varName,
                                         const void *initData)
{
    // duplicate variable declaration
    auto pVar = searchVariableCurrentScope(varName);
    if (pVar)
        throw duplicateVariableName(varName);

    auto typeNum = getTypeMgr().getTypeNumByName(typeName);
    auto type = getTypeMgr().getTypeByNum(typeNum);

    // insert new variable
    std::unique_ptr<uint8_t[]> data(new uint8_t[type.getSize()]);
    memcpy(data.get(), initData, type.getSize());
    varStack.back().emplace(varName,
                            VariableInfo{typeNum, type.getSize(), data.get()});
    data.release();
}

void VariableManager::assignVariable(const std::string &varName,
                                     const void *data)
{
    auto pVar = searchVariableRecursive(varName);
    if (!pVar)
        throw unknownVariableName(varName);
    memcpy(pVar->data, data, pVar->typeSize);
}

void *VariableManager::getVariableData(const std::string &varName)
{
    auto pVar = searchVariableRecursive(varName);
    if (!pVar)
        throw unknownVariableName(varName);
    return pVar->data;
}


int VariableManager::getVariableTypeNum(const std::string &varName)
{
    auto pVar = searchVariableRecursive(varName);
    if (!pVar)
        throw unknownVariableName(varName);
    return pVar->typeNum;
}

void VariableManager::popScope()
{
    for (auto item : varStack.back())
        delete[] reinterpret_cast<uint8_t*>(item.second.data);
    varStack.pop_back();
}

}  // namespace cint
