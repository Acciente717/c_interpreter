/* Copyright @2019 Zhiyao Ma */
#include <cstdint>
#include <memory>
#include <cstring>

#include "variableManager.hh"
#include "exceptions.hh"

namespace cint
{

VariableInfoBase::~VariableInfoBase()
{
    if (!isReference)
        delete[] reinterpret_cast<uint8_t*>(data);
}

VariableInfoBase::VariableInfoBase(
    decltype(VariableInfoBase::baseTypeNum) _baseTypeNum,
    decltype(VariableInfoBase::baseTypeSize) _baseTypeSize,
    decltype(VariableInfoBase::data) _data,
    decltype(VariableInfoBase::isReference) _isReference,
    decltype(VariableInfoBase::isTemporary) _isTemporary) noexcept
  : baseTypeNum(_baseTypeNum), baseTypeSize(_baseTypeSize),
    data(_data), isReference(_isReference), isTemporary(_isTemporary) {}

VariableInfoBase::VariableInfoBase(
    VariableInfoBase &&other) noexcept
  : baseTypeNum(other.baseTypeNum), baseTypeSize(other.baseTypeSize),
    data(other.data), isReference(other.isReference),
    isTemporary(other.isTemporary)
{
    other.data = nullptr;
}

VariableInfoBase& VariableInfoBase::operator=(
    VariableInfoBase &&other) noexcept
{
    delete[] reinterpret_cast<uint8_t*>(data);
    baseTypeNum = other.baseTypeNum;
    baseTypeSize = other.baseTypeSize;
    data = other.data;
    isReference = other.isReference;
    isTemporary = other.isTemporary;
    other.data = nullptr;
    return *this;
}

decltype(VariableInfoSolid::baseTypeNum)
    VariableInfoSolid::getTypeNum() const noexcept
{
    return baseTypeNum;
}

decltype(VariableInfoSolid::baseTypeSize)
    VariableInfoSolid::getTypeSize() const noexcept
{
    return baseTypeSize;
}

const decltype(VariableInfoSolid::data)
    VariableInfoSolid::getData() const noexcept
{
    return data;
}

decltype(VariableInfoSolid::data)
    VariableInfoSolid::getMutableData() noexcept
{
    return data;
}

decltype(VariableInfoSolid::isReference)
    VariableInfoSolid::getIsReference() const noexcept
{
    return isReference;
}

void VariableInfoSolid::updateData(const void *new_data)
{
    memcpy(data, new_data, baseTypeSize);
}

void VariableInfoSolid::setReference(void *new_ref)
{
    throw badVariableOperation("VariableInfoSolid::setReference");
}


VariableInfoSolid::VariableInfoSolid(
    decltype(baseTypeNum) _baseTypeNum,
    decltype(baseTypeSize) _baseTypeSize,
    decltype(data) _data,
    decltype(isTemporary) _isTemporary) noexcept
  : VariableInfoBase(_baseTypeNum, _baseTypeSize, _data, false, _isTemporary)
{ }

VariableInfoSolid::VariableInfoSolid(VariableInfoSolid &&other) noexcept
  : VariableInfoBase(std::move(other))
{ }

VariableInfoSolid& VariableInfoSolid::operator=(
    VariableInfoSolid &&other) noexcept
{
    VariableInfoSolid::operator=(std::move(other));
    return *this;
}

VariableInfoSolid::~VariableInfoSolid()
{ }

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

VariableInfoBase *VariableManager::searchVariableRecursive(
    const std::string &varName
)
{
    for (int i = varStack.size() - 1; i >= 0; --i)
    {
        auto iter = varStack[i].find(varName);
        if (iter != varStack[i].end())
            return iter->second.get();
        if (funcIndicatorStack[i])
            break;
    }
    auto iter = globals.find(varName);
    if (iter != globals.end())
        return iter->second.get();
    return nullptr;
}

void VariableManager::declareVariable(const std::string &typeName,
                                      const std::string &varName,
                                      bool isTemporary)
{
    // duplicate variable declaration
    auto pVar = searchVariableCurrentScope(varName);
    if (pVar)
        throw duplicateVariableName(varName);

    auto typeNum = getTypeMgr().getTypeNumByName(typeName);
    auto type = getTypeMgr().getTypeByNum(typeNum);

    // insert new variable
    std::unique_ptr<uint8_t[]> data(new uint8_t[type.getSize()]);
    varStack.back().emplace(
        varName,
        std::make_unique<VariableInfoSolid>
            (typeNum, type.getSize(), data.get(), isTemporary));
    data.release();
}


void VariableManager::initializeVariable(const std::string &typeName,
                                         const std::string &varName,
                                         const void *initData,
                                         bool isTemporary)
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
    varStack.back().emplace(
        varName,
        std::make_unique<VariableInfoSolid>
            (typeNum, type.getSize(), data.get(), isTemporary));
    data.release();
}

void VariableManager::assignVariable(const std::string &varName,
                                     const void *data)
{
    auto pVar = searchVariableRecursive(varName);
    if (!pVar)
        throw unknownVariableName(varName);
    pVar->updateData(data);
}

// const void *VariableManager::getVariableData(const std::string &varName)
// {
//     auto pVar = searchVariableRecursive(varName);
//     if (!pVar)
//         throw unknownVariableName(varName);
//     return pVar->getData();
// }

VariableInfoBase *VariableManager::getVariableInfo(const std::string &varName)
{
    auto pVar = searchVariableRecursive(varName);
    if (!pVar)
        throw unknownVariableName(varName);
    return pVar;
}


int VariableManager::getVariableTypeNum(const std::string &varName)
{
    auto pVar = searchVariableRecursive(varName);
    if (!pVar)
        throw unknownVariableName(varName);
    return pVar->getTypeNum();
}

void VariableManager::popScope()
{
    varStack.pop_back();
    funcIndicatorStack.pop_back();
}

void VariableManager::updateReturnValue(const std::string &typeName,
                                        const void *updateData)
{
    auto typeNum = getTypeMgr().getTypeNumByName(typeName);
    auto type = getTypeMgr().getTypeByNum(typeNum);

    // update return value
    std::unique_ptr<uint8_t[]> data(new uint8_t[type.getSize()]);
    memcpy(data.get(), updateData, type.getSize());
    pReturnValue = std::make_unique<VariableInfoSolid>
                    (typeNum, type.getSize(), data.get(), true);
    data.release();
}

const void *VariableManager::getReturnValueData()
{
    return pReturnValue->getData();
}

int VariableManager::getReturnValueTypeNum()
{
    return pReturnValue->getTypeNum();
}

void VariableManager::setReturnValueToVoid()
{
    std::unique_ptr<uint8_t[]> data(new uint8_t[basicTypesSize[CVOID]]);
    pReturnValue = std::make_unique<VariableInfoSolid>
                    (CVOID, basicTypesSize[CVOID], data.get(), true);
    data.release();
}

}  // namespace cint
