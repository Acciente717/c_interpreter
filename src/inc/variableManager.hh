/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_VARIABLE_MANAGER_HH_
#define SRC_INC_VARIABLE_MANAGER_HH_

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

#include "typeManager.hh"

namespace cint
{

/*************************************************
*               Type Declarations                *
*************************************************/

class VariableInfoBase
{
 protected:
    long baseTypeNum;
    unsigned baseTypeSize;
    void *data;
    bool isReference;
    bool isTemporary;

 protected:
    inline VariableInfoBase(decltype(baseTypeNum) _baseTypeNum,
                            decltype(data) _data,
                            decltype(isReference) _isReference,
                            decltype(isTemporary) _isTemporary) noexcept;
    inline VariableInfoBase(VariableInfoBase &&other) noexcept;
    inline VariableInfoBase& operator=(VariableInfoBase &&other) noexcept;

 public:
    VariableInfoBase() = delete;
    VariableInfoBase(const VariableInfoBase &other) = delete;
    VariableInfoBase& operator=(const VariableInfoBase &other) = delete;
    virtual ~VariableInfoBase();

    virtual decltype(baseTypeNum) getTypeNum() const noexcept = 0;
    virtual decltype(baseTypeSize) getTypeSize() const noexcept = 0;
    virtual const void *getData() const noexcept = 0;
    virtual decltype(data) getMutableData() = 0;
    virtual decltype(isReference) getIsReference() const noexcept = 0;
    virtual void updateData(const void *new_data) = 0;
    virtual void setReference(void *new_ref) = 0;
    virtual const void *getReference() = 0;
};

class VariableInfoArray : public VariableInfoBase
{
 protected:
    std::vector<long> dimSizes;
    ssize_t shift;
    long topLevelSize;
    bool isLeaf;

 public:
    VariableInfoArray() = delete;
    VariableInfoArray(const VariableInfoArray &other) = delete;
    VariableInfoArray& operator=(const VariableInfoArray &other) = delete;
    explicit VariableInfoArray(decltype(baseTypeNum) _baseTypeNum,
                               decltype(data) _data,
                               const decltype(dimSizes) &_dimSizes,
                               decltype(topLevelSize) _topLevelSize,
                               decltype(isReference) _isReference);
    explicit VariableInfoArray(decltype(baseTypeNum) _baseTypeNum,
                               decltype(data) _data,
                               const decltype(dimSizes) &_shape,
                               decltype(isReference) _isReference);
    explicit VariableInfoArray(VariableInfoArray &&other) noexcept;
    VariableInfoArray& operator=(VariableInfoArray &&other) noexcept;
    ~VariableInfoArray() override;

    decltype(baseTypeNum) getTypeNum() const noexcept override;
    decltype(baseTypeSize) getTypeSize() const noexcept override;
    const void *getData() const noexcept override;
    decltype(data) getMutableData() override;
    decltype(isReference) getIsReference() const noexcept override;
    void updateData(const void *new_data) override;
    void setReference(void *new_ref) override;
    const void *getReference() override;

    std::unique_ptr<VariableInfoArray>
        address(const decltype(dimSizes) &indicies);
};

class VariableInfoSolid : public VariableInfoBase
{
 public:
    VariableInfoSolid() = delete;
    VariableInfoSolid(const VariableInfoSolid &other) = delete;
    VariableInfoSolid& operator=(const VariableInfoSolid &other) = delete;
    explicit VariableInfoSolid(decltype(baseTypeNum) _baseTypeNum,
                               decltype(data) _data,
                               decltype(isTemporary) _isTemporary) noexcept;
    explicit VariableInfoSolid(VariableInfoSolid &&other) noexcept;
    VariableInfoSolid& operator=(VariableInfoSolid &&other) noexcept;
    ~VariableInfoSolid() override;

    decltype(baseTypeNum) getTypeNum() const noexcept override;
    decltype(baseTypeSize) getTypeSize() const noexcept override;
    const void *getData() const noexcept override;
    decltype(data) getMutableData() override;
    decltype(isReference) getIsReference() const noexcept override;
    void updateData(const void *new_data) override;
    void setReference(void *new_ref) override;
    const void *getReference() override;
};

using variableDictionary =
    std::unordered_map<std::string,
                       std::unique_ptr<VariableInfoBase> >;

class VariableManager
{
    std::vector<variableDictionary> varStack;
    std::vector<uint8_t> funcIndicatorStack;
    variableDictionary globals;
    std::unique_ptr<VariableInfoBase> pReturnValue;

    inline VariableInfoBase *searchVariableCurrentScope(
        const std::string &varName
    );

    VariableInfoBase *searchVariableRecursive(
        const std::string &varName
    );
 public:
    inline VariableManager();
    ~VariableManager() noexcept;

    inline void newScope(bool isFunction = false);
    inline void setScopeBoundary();
    void popScope();
    void declareVariable(const std::string &typeName,
                         const std::string &varName,
                         bool isTemporary);
    void initializeVariable(const std::string &typeName,
                            const std::string &varName,
                            const void *initData,
                            bool isTemporary);
    void assignVariable(const std::string &varName, const void *data);
    void declareArrayVariable(const std::string &baseTypeName,
                              const std::string &varName,
                              const std::vector<long> &shape,
                              bool isTemporary);
    void moveInArrayVariable(
        const std::string &varName,
        std::unique_ptr<VariableInfoArray> newArrayVar);
    // const void *getVariableData(const std::string &varName);
    VariableInfoBase *getVariableInfo(const std::string &varName);
    long getVariableTypeNum(const std::string &varName);
    void updateReturnValue(const std::string &typeName,
                           const void *updateData);
    const void *getReturnValueData();
    long getReturnValueTypeNum();
    void setReturnValueToVoid();
};

VariableManager& getVarMgr();

/*************************************************
*              Type Implementations              *
*************************************************/

inline VariableManager::VariableManager()
{
    pReturnValue = std::make_unique<VariableInfoSolid>(
        CVOID, new uint8_t[1], true
    );
    varStack.emplace_back();
    funcIndicatorStack.push_back(0);
}

inline VariableInfoBase* VariableManager::searchVariableCurrentScope(
    const std::string &varName)
{
    auto iter = varStack.back().find(varName);
    if (iter == varStack.back().end())
        return nullptr;
    return iter->second.get();
}

inline void VariableManager::newScope(bool isFunction)
{
    varStack.emplace_back();
    funcIndicatorStack.push_back(static_cast<uint8_t>(isFunction));
}

inline void VariableManager::setScopeBoundary()
{
    funcIndicatorStack.back() = 1;
}

}  // namespace cint

#endif  // SRC_INC_VARIABLE_MANAGER_HH_
