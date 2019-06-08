/* Copyright @2019 Zhiyao Ma */
#include <cstdint>
#include <memory>
#include <cstring>

#include "variableManager.hh"
#include "exceptions.hh"
#include "representations.hh"

namespace cint
{

VariableInfoBase::~VariableInfoBase()
{
    if (!isReference)
        delete[] reinterpret_cast<uint8_t*>(data);
}

VariableInfoBase::VariableInfoBase(
    decltype(VariableInfoBase::baseTypeNum) _baseTypeNum,
    decltype(VariableInfoBase::data) _data,
    decltype(VariableInfoBase::isReference) _isReference,
    decltype(VariableInfoBase::isTemporary) _isTemporary) noexcept
  : baseTypeNum(_baseTypeNum), data(_data),
    isReference(_isReference), isTemporary(_isTemporary)
{
    baseTypeSize = getTypeMgr().getSizeByNum(_baseTypeNum);
}

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

decltype(VariableInfoArray::baseTypeNum)
    VariableInfoArray::getTypeNum() const noexcept
{
    if (isLeaf)
        return baseTypeNum;
    return CARRAY;
}

decltype(VariableInfoArray::baseTypeSize)
    VariableInfoArray::getTypeSize() const noexcept
{
    if (isLeaf)
        return baseTypeSize;
    return topLevelSize * dimSizes[0];
}

const void *VariableInfoArray::getData() const noexcept
{
    if (isLeaf)
        return data;
    return &data;
}

decltype(VariableInfoArray::data)
    VariableInfoArray::getMutableData()
{
    if (isLeaf)
        return data;
    throw badVariableOperation("VariableInfoArray::getMutableData "
                               "invoked on non-leaf array variable");
}

decltype(VariableInfoArray::isReference)
    VariableInfoArray::getIsReference() const noexcept
{
    return isReference;
}

void VariableInfoArray::updateData(const void *new_data)
{
    if (isLeaf)
        memcpy(data, new_data, baseTypeSize);
    else
        throw badVariableOperation("VariableInfoArray::updateData");
}

void VariableInfoArray::setReference(void *new_ref)
{
    throw badVariableOperation("VariableInfoArray::setReference");
}

VariableInfoArray::VariableInfoArray(
    decltype(baseTypeNum) _baseTypeNum,
    decltype(data) _data,
    const decltype(dimSizes) &_dimSizes,
    decltype(topLevelSize) _topLevelSize,
    decltype(isReference) _isReference)
  : VariableInfoBase(_baseTypeNum, _data, _isReference, false)
{
    if (_dimSizes.empty())
    {
        isLeaf = true;
        shift = 0;
        topLevelSize = 0;
    }
    else
    {
        isLeaf = false;
        shift = 0;
        dimSizes = _dimSizes;
        topLevelSize = _topLevelSize;
    }
}

VariableInfoArray::VariableInfoArray(
    decltype(baseTypeNum) _baseTypeNum,
    decltype(data) _data,
    const decltype(dimSizes) &_shape,
    decltype(isReference) _isReference)
  : VariableInfoBase(_baseTypeNum, _data, _isReference, false)
{
    if (unlikely(_shape.empty()))
        throw badVariableOperation("VariableInfoArray::address "
                                   "empty shape list");
    isLeaf = false;
    shift = 0;
    dimSizes.resize(_shape.size(), 0);
    ssize_t i = _shape.size() - 1;
    dimSizes[i] = baseTypeSize;
    for (--i; i >= 0; --i)
        dimSizes[i] = dimSizes[i + 1] * _shape[i + 1];
    topLevelSize = _shape[0];
}

VariableInfoArray::VariableInfoArray(VariableInfoArray &&other) noexcept
  : VariableInfoBase(std::move(other))
{
    shift = other.shift;
    topLevelSize = other.topLevelSize;
    isLeaf = other.isLeaf;
    dimSizes = std::move(other.dimSizes);
}

VariableInfoArray& VariableInfoArray::operator=(
    VariableInfoArray &&other) noexcept
{
    VariableInfoArray::operator=(std::move(other));
    shift = other.shift;
    isLeaf = other.isLeaf;
    dimSizes = std::move(other.dimSizes);
    return *this;
}


const void *VariableInfoArray::getReference() const
{
    return data;
}

VariableInfoArray::~VariableInfoArray()
{ }

std::unique_ptr<VariableInfoArray>
VariableInfoArray::address(const decltype(dimSizes) &indicies)
{
    if (unlikely(indicies.size() > dimSizes.size()))
        throw badVariableOperation("VariableInfoArray::address "
                                   "variable not addressable by "
                                   "input indicies");
    if (unlikely(indicies.empty()))
        throw badVariableOperation("VariableInfoArray::address "
                                   "empty indicies list");

    ssize_t shiftVal = 0;
    for (auto i = 0; i < indicies.size(); ++i)
        shiftVal += dimSizes[i] * indicies[i];

    return std::make_unique<VariableInfoArray>(
        baseTypeNum, reinterpret_cast<uint8_t *>(data) + shiftVal,
        decltype(dimSizes){dimSizes.begin() + indicies.size(), dimSizes.end()},
        0, true
    );
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

const void *VariableInfoSolid::getData() const noexcept
{
    return data;
}

decltype(VariableInfoSolid::data)
    VariableInfoSolid::getMutableData()
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
    decltype(data) _data,
    decltype(isTemporary) _isTemporary) noexcept
  : VariableInfoBase(_baseTypeNum, _data, false, _isTemporary)
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


const void *VariableInfoSolid::getReference() const
{
    throw badVariableOperation("VariableInfoSolid::getReference");
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
    for (long i = varStack.size() - 1; i >= 0; --i)
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
            (typeNum, data.get(), isTemporary));
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
            (typeNum, data.get(), isTemporary));
    data.release();
}

void VariableManager::declareArrayVariable(
    const std::string &baseTypeName,
    const std::string &varName,
    const std::vector<long> &shape,
    bool isTemporary)
{
    // duplicate variable declaration
    auto pVar = searchVariableCurrentScope(varName);
    if (pVar)
        throw duplicateVariableName(varName);

    auto baseTypeNum = getTypeMgr().getTypeNumByName(baseTypeName);
    auto type = getTypeMgr().getTypeByNum(baseTypeNum);

    size_t totalSize = 1;
    for (auto i : shape)
        totalSize *= i;
    totalSize *= type.getSize();

    // insert new variable
    std::unique_ptr<uint8_t[]> data(new uint8_t[totalSize]);
    varStack.back().emplace(
        varName,
        std::make_unique<VariableInfoArray>
            (baseTypeNum, data.get(), shape, isTemporary));
    data.release();
}

void VariableManager::moveInArrayVariable(
    const std::string &varName,
    std::unique_ptr<VariableInfoArray> newArrayVar
)
{
    // duplicate variable declaration
    auto pVar = searchVariableCurrentScope(varName);
    if (pVar)
        throw duplicateVariableName(varName);

    varStack.back()[varName] = std::move(newArrayVar);
}


void VariableManager::initializeArrayArgument(
    const VariableInfoArray *refArray,
    const std::string &baseTypeName,
    const std::string &varName,
    const std::vector<long> &shape)
{
    // duplicate variable declaration
    auto pVar = searchVariableCurrentScope(varName);
    if (pVar)
        throw duplicateVariableName(varName);

    auto baseTypeNum = getTypeMgr().getTypeNumByName(baseTypeName);
    auto type = getTypeMgr().getTypeByNum(baseTypeNum);

    varStack.back().emplace(
        varName,
        std::make_unique<VariableInfoArray>(
            baseTypeNum,
            const_cast<void *>(refArray->getReference()),
            shape, true));
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


long VariableManager::getVariableTypeNum(const std::string &varName)
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
                    (typeNum, data.get(), true);
    data.release();
}

const void *VariableManager::getReturnValueData()
{
    return pReturnValue->getData();
}

long VariableManager::getReturnValueTypeNum()
{
    return pReturnValue->getTypeNum();
}

void VariableManager::setReturnValueToVoid()
{
    std::unique_ptr<uint8_t[]> data(new uint8_t[basicTypesSize[CVOID]]);
    pReturnValue = std::make_unique<VariableInfoSolid>
                    (CVOID, data.get(), true);
    data.release();
}

}  // namespace cint
