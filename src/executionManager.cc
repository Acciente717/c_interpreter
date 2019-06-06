/* Copyright @2019 Zhiyao Ma */
#include <thread>
#include <chrono>

#include "utils.hh"
#include "executionManager.hh"
#include "functionManager.hh"

namespace cint
{

static std::vector<int> decideTypeConversionTernary(
    const ternaryOperation *pOpr
)
{
    auto xType = getVarMgr().getVariableTypeNum(pOpr->vars[0]);
    auto yType = getVarMgr().getVariableTypeNum(pOpr->vars[1]);
    auto zType = getVarMgr().getVariableTypeNum(pOpr->vars[2]);
    auto res = std::vector<int>(3);

    switch (pOpr->oprType)
    {
    // arithmic operations
    case ternaryOprType::assignSum:
    case ternaryOprType::assignDifference:
    case ternaryOprType::assignProduct:
    case ternaryOprType::assignQuotient:
    case ternaryOprType::assignResidue:

        // determine type conversion before calculation
        if (yType == zType)
            res[1] = res[2] = yType;
        else
            res[1] = res[2] = typeManager::chooseSuperType(yType, zType);

        break;

    // logic operations
    case ternaryOprType::assignLess:
    case ternaryOprType::assignLessEqual:
    case ternaryOprType::assignGreater:
    case ternaryOprType::assignGreaterEqual:
    case ternaryOprType::assignEqual:
    case ternaryOprType::assignNotEqual:
    case ternaryOprType::assignLogicAnd:
    case ternaryOprType::assignLogicOr:

        // determine type conversion before calculation
        if (yType == zType)
            res[1] = res[2] = yType;
        else
            res[1] = res[2] = typeManager::chooseSuperType(yType, zType);

        break;
    default:
        throw unknownSwitchCase("decideTypeConversionTernary");
    }
    return res;
}

static std::unique_ptr<uint8_t[]> createConvertedVariable(
    int tgtTypeNum, int srcTypeNum, const void *data
)
{
    std::unique_ptr<uint8_t[]>
        new_data(new uint8_t[basicTypesSize[tgtTypeNum]]);

    switch (tgtTypeNum)
    {
    case CINT32:
        switch (srcTypeNum)
        {
        case CINT32:
            *reinterpret_cast<int32_t*>(new_data.get())
                = *reinterpret_cast<const int32_t*>(data);
            break;
        case CFLOAT:
            *reinterpret_cast<int32_t*>(new_data.get())
                = *reinterpret_cast<const float*>(data);
            break;
        default:
            throw unknownSwitchCase("createConvertedVariable");
        }
        break;
    case CFLOAT:
        switch (srcTypeNum)
        {
        case CINT32:
            *reinterpret_cast<float*>(new_data.get())
                = *reinterpret_cast<const int32_t*>(data);
            break;
        case CFLOAT:
            *reinterpret_cast<float*>(new_data.get())
                = *reinterpret_cast<const float*>(data);
            break;
        default:
            throw unknownSwitchCase("createConvertedVariable");
        }
        break;
    case CVOID:
        switch (srcTypeNum)
        {
        case CVOID:
            *reinterpret_cast<float*>(new_data.get())
                = *reinterpret_cast<const int32_t*>(data);
            break;
        default:
            throw unknownSwitchCase("createConvertedVariable");
        }
        break;
    default:
        throw unknownSwitchCase("createConvertedVariable");
    }
    return new_data;
}

inline void int32Arithmic(ternaryOprType oprNum, void *px,
                          const void *py, const void *pz)
{
    auto *pIntX = reinterpret_cast<int32_t*>(px);
    auto *pIntY = reinterpret_cast<const int32_t*>(py);
    auto *pIntZ = reinterpret_cast<const int32_t*>(pz);

    switch (oprNum)
    {
    case ternaryOprType::assignSum:
        *pIntX = *pIntY + *pIntZ;
        break;
    case ternaryOprType::assignDifference:
        *pIntX = *pIntY - *pIntZ;
        break;
    case ternaryOprType::assignProduct:
        *pIntX = *pIntY * *pIntZ;
        break;
    case ternaryOprType::assignQuotient:
        *pIntX = *pIntY / *pIntZ;
        break;
    case ternaryOprType::assignResidue:
        *pIntX = *pIntY % *pIntZ;
        break;
    case ternaryOprType::assignLess:
        *pIntX = static_cast<int32_t>(*pIntY < *pIntZ);
        break;
    case ternaryOprType::assignLessEqual:
        *pIntX = static_cast<int32_t>(*pIntY <= *pIntZ);
        break;
    case ternaryOprType::assignGreater:
        *pIntX = static_cast<int32_t>(*pIntY > *pIntZ);
        break;
    case ternaryOprType::assignGreaterEqual:
        *pIntX = static_cast<int32_t>(*pIntY >= *pIntZ);
        break;
    case ternaryOprType::assignEqual:
        *pIntX = static_cast<int32_t>(*pIntY == *pIntZ);
        break;
    case ternaryOprType::assignNotEqual:
        *pIntX = static_cast<int32_t>(*pIntY != *pIntZ);
        break;
    case ternaryOprType::assignLogicAnd:
        *pIntX = static_cast<int32_t>(*pIntY && *pIntZ);
        break;
    case ternaryOprType::assignLogicOr:
        *pIntX = static_cast<int32_t>(*pIntY || *pIntZ);
        break;
    default:
        throw unknownSwitchCase("int32Arithmic");
    }
}

inline void floatArithmic(ternaryOprType oprNum, void *px,
                          const void *py, const void *pz)
{
    auto *pFloatX = reinterpret_cast<float*>(px);
    auto *pFloatY = reinterpret_cast<const float*>(py);
    auto *pFloatZ = reinterpret_cast<const float*>(pz);

    switch (oprNum)
    {
    case ternaryOprType::assignSum:
        *pFloatX = *pFloatY + *pFloatZ;
        break;
    case ternaryOprType::assignDifference:
        *pFloatX = *pFloatY - *pFloatZ;
        break;
    case ternaryOprType::assignProduct:
        *pFloatX = *pFloatY * *pFloatZ;
        break;
    case ternaryOprType::assignQuotient:
        *pFloatX = *pFloatY / *pFloatZ;
        break;
    case ternaryOprType::assignResidue:
        throw unsupportedArithmic("cannot apply moduolo to floats");
        break;
    case ternaryOprType::assignLess:
        *pFloatX = static_cast<int32_t>(*pFloatY < *pFloatZ);
        break;
    case ternaryOprType::assignLessEqual:
        *pFloatX = static_cast<int32_t>(*pFloatY <= *pFloatZ);
        break;
    case ternaryOprType::assignGreater:
        *pFloatX = static_cast<int32_t>(*pFloatY > *pFloatZ);
        break;
    case ternaryOprType::assignGreaterEqual:
        *pFloatX = static_cast<int32_t>(*pFloatY >= *pFloatZ);
        break;
    case ternaryOprType::assignEqual:
        *pFloatX = static_cast<int32_t>(*pFloatY == *pFloatZ);
        break;
    case ternaryOprType::assignNotEqual:
        *pFloatX = static_cast<int32_t>(*pFloatY != *pFloatZ);
        break;
    case ternaryOprType::assignLogicAnd:
        *pFloatX = static_cast<int32_t>(*pFloatY && *pFloatZ);
        break;
    case ternaryOprType::assignLogicOr:
        *pFloatX = static_cast<int32_t>(*pFloatY || *pFloatZ);
        break;
    default:
        throw unknownSwitchCase("floatArithmic");
    }
}

std::vector<int> prepareIndicies(const std::vector<std::string> &charIndicies)
{
    std::vector<int> res;
    for (const auto &i : charIndicies)
    {
        auto info = getVarMgr().getVariableInfo(i);
        if (unlikely(info->getTypeNum() != CINT32))
            throw badIntermediateCommand("prepareIndicies: indicies all are"
                                         " not in type `int`");
        res.push_back(*reinterpret_cast<const int*>(info->getData()));
    }
    return res;
}

void executionManager::exeUnaryOpr(const unaryOperation *pOpr)
{
    decltype(getVarMgr().getVariableInfo("")) xInfo;

    switch (pOpr->oprType)
    {
    case unaryOprType::getReturnValue:
        // If x is a temporary variable, we must first declare it to the
        // variable manager.
        if (pOpr->vars[0][0] == '#')
        {
            getVarMgr().declareVariable(
            getTypeMgr().getTypenameByNum(getVarMgr().getReturnValueTypeNum()),
            pOpr->vars[0], true);
        }
        xInfo = getVarMgr().getVariableInfo(pOpr->vars[0]);
        if (xInfo->getTypeNum() != getVarMgr().getReturnValueTypeNum())
        {
            auto temp = createConvertedVariable(
                xInfo->getTypeNum(),
                getVarMgr().getReturnValueTypeNum(),
                getVarMgr().getReturnValueData());
            xInfo->updateData(temp.get());
        }
        else
        {
            xInfo->updateData(getVarMgr().getReturnValueData());
        }
        break;
    default:
        throw unknownSwitchCase("executionManager::exeUnaryOpr");
    }
}

void executionManager::exeBinaryOpr(const binaryOperation *pOpr)
{
    decltype(getVarMgr().getVariableInfo("")) xInfo, yInfo;

    switch (pOpr->oprType)
    {
    case binaryOprType::assignVariable:
        xInfo = getVarMgr().getVariableInfo(pOpr->vars[0]);
        yInfo = getVarMgr().getVariableInfo(pOpr->vars[1]);
        if (xInfo->getData() == yInfo->getData())
            break;
        if (xInfo->getTypeNum() != yInfo->getTypeNum())
        {
            auto temp = createConvertedVariable(
                xInfo->getTypeNum(),
                yInfo->getTypeNum(),
                yInfo->getData());
            xInfo->updateData(temp.get());
        }
        else
        {
            xInfo->updateData(yInfo->getData());
        }
        break;
    case binaryOprType::assignLiteral:
        xInfo = getVarMgr().getVariableInfo(pOpr->vars[0]);
        switch (xInfo->getTypeNum())
        {
        case CINT32:
            {
                auto value = std::stoi(pOpr->vars[1]);
                xInfo->updateData(&value);
            }
            break;
        case CFLOAT:
            {
                auto value = std::stof(pOpr->vars[1]);
                xInfo->updateData(&value);
            }
            break;
        default:
            throw unknownSwitchCase("executionManager::exeBinaryOpr");
        }
        break;
    case binaryOprType::assignNot:

        // If x is a temporary variable, we must first declare it to the
        // variable manager.
        if (pOpr->vars[0][0] == '#')
        {
            getVarMgr().declareVariable(
                getTypeMgr().getTypenameByNum(CINT32),
                pOpr->vars[0], true);
        }

        xInfo = getVarMgr().getVariableInfo(pOpr->vars[0]);
        yInfo = getVarMgr().getVariableInfo(pOpr->vars[1]);
        switch (yInfo->getTypeNum())
        {
        case CINT32:
            *reinterpret_cast<int32_t *>(xInfo->getMutableData()) =
                !*reinterpret_cast<const int32_t *>(yInfo->getData());
            break;
        case CFLOAT:
            *reinterpret_cast<int32_t *>(xInfo->getMutableData()) =
                !*reinterpret_cast<const float *>(yInfo->getData());
            break;
        default:
            throw unknownSwitchCase("executionManager::exeBinaryOpr");
        }
        break;
    case binaryOprType::assignNegate:
        yInfo = getVarMgr().getVariableInfo(pOpr->vars[1]);

        // If x is a temporary variable, we must first declare it to the
        // variable manager.
        if (pOpr->vars[0][0] == '#')
        {
            getVarMgr().declareVariable(
                getTypeMgr().getTypenameByNum(yInfo->getTypeNum()),
                pOpr->vars[0], true);
        }
        xInfo = getVarMgr().getVariableInfo(pOpr->vars[0]);

        switch (yInfo->getTypeNum())
        {
        case CINT32:
            *reinterpret_cast<int32_t *>(xInfo->getMutableData()) =
                -*reinterpret_cast<const int32_t *>(yInfo->getData());
            break;
        case CFLOAT:
            *reinterpret_cast<float *>(xInfo->getMutableData()) =
                -*reinterpret_cast<const float *>(yInfo->getData());
            break;
        default:
            throw unknownSwitchCase("executionManager::exeBinaryOpr");
        }
        break;
    default:
        throw unknownSwitchCase("executionManager::exeBinaryOpr");
    }
}

void executionManager::exeTernaryOpr(const ternaryOperation *pOpr)
{
    decltype(getVarMgr().getVariableInfo("")) yInfo, zInfo;
    const void *yPtr, *zPtr;
    void *tgtPtr;

    std::unique_ptr<uint8_t[]> xSmart, ySmart, zSmart;
    int superType;

    yInfo = getVarMgr().getVariableInfo(pOpr->vars[1]);
    zInfo = getVarMgr().getVariableInfo(pOpr->vars[2]);

    // if y and z have different types, convert them before proceeding
    if (yInfo->getTypeNum() != zInfo->getTypeNum())
    {
        superType = typeManager::chooseSuperType(
            yInfo->getTypeNum(),
            zInfo->getTypeNum());
        if (yInfo->getTypeNum() != superType)
        {
            ySmart = createConvertedVariable(superType, yInfo->getTypeNum(),
                                             yInfo->getData());
            yPtr = ySmart.get();
        }
        if (zInfo->getTypeNum() != superType)
        {
            zSmart = createConvertedVariable(superType, zInfo->getTypeNum(),
                                             zInfo->getData());
            zPtr = zSmart.get();
        }
    }
    else
    {
        superType = yInfo->getTypeNum();
        yPtr = yInfo->getData();
        zPtr = zInfo->getData();
    }

    // If x is a temporary variable, we must first declare it to the
    // variable manager.
    if (pOpr->vars[0][0] == '#')
    {
        getVarMgr().declareVariable(
            getTypeMgr().getTypenameByNum(superType),
            pOpr->vars[0], true);
    }

    auto xInfo = getVarMgr().getVariableInfo(pOpr->vars[0]);

    // If x has different types from (y OP z), create a temporary variable
    // for x. For arithmic operations, create a temporaty variable with
    // the same type as superType. For logical operations, create a variable
    // with the time int32_t.
    switch (pOpr->oprType)
    {
    // arithmic operations
    case ternaryOprType::assignSum:
    case ternaryOprType::assignDifference:
    case ternaryOprType::assignProduct:
    case ternaryOprType::assignQuotient:
    case ternaryOprType::assignResidue:
        if (xInfo->getTypeNum() != superType)
        {
            xSmart.reset(new uint8_t[basicTypesSize[superType]]);
            tgtPtr = xSmart.get();
        }
        else
        {
            tgtPtr = xInfo->getMutableData();
        }
        break;

    // logical operations
    case ternaryOprType::assignLess:
    case ternaryOprType::assignLessEqual:
    case ternaryOprType::assignGreater:
    case ternaryOprType::assignGreaterEqual:
    case ternaryOprType::assignEqual:
    case ternaryOprType::assignNotEqual:
    case ternaryOprType::assignLogicAnd:
    case ternaryOprType::assignLogicOr:
        if (xInfo->getTypeNum() != CINT32)
        {
            xSmart.reset(new uint8_t[basicTypesSize[CINT32]]);
            tgtPtr = xSmart.get();
        }
        else
        {
            tgtPtr = xInfo->getMutableData();
        }
        break;
    default:
        throw unknownSwitchCase("executionManager::exeBinaryOpr");
    }

    // dispatch operation to specific functions according to data types
    switch (superType)
    {
    case CINT32:
        int32Arithmic(pOpr->oprType, tgtPtr, yPtr, zPtr);
        break;
    case CFLOAT:
        floatArithmic(pOpr->oprType, tgtPtr, yPtr, zPtr);
        break;
    default:
        throw unknownSwitchCase("executionManager::exeBinaryOpr");
    }

    // if we previous write to the temporary variable, convert and
    // write back to x
    if (xInfo->getTypeNum() != superType)
    {
        xSmart = createConvertedVariable(
            xInfo->getTypeNum(), superType, xSmart.get());
        xInfo->updateData(xSmart.get());
    }
}

void executionManager::exeReadArrOpr(const readArrayOperation *pOpr)
{
    auto rawInfo = getVarMgr().getVariableInfo(pOpr->arName);
    if (unlikely(rawInfo->getTypeNum() != CARRAY))
        throw badIntermediateCommand("executionManager::exeReadArrOpr: "
                                     "read array operation is performing"
                                     " on a non-array variable");
    auto arInfo = static_cast<VariableInfoArray *>(rawInfo);
    auto intVec = prepareIndicies(pOpr->idxs);

    auto readedVarInfo = arInfo->address(intVec);

    // we get a single element from the array
    if (readedVarInfo->getTypeNum() != CARRAY)
    {
        // If x is a temporary variable, we must first declare it to the
        // variable manager.
        if (pOpr->tgtName[0] == '#')
        {
            getVarMgr().declareVariable(
                getTypeMgr().getTypenameByNum(readedVarInfo->getTypeNum()),
                pOpr->tgtName, true);
        }

        auto tgtInfo = getVarMgr().getVariableInfo(pOpr->tgtName);
        if (tgtInfo->getTypeNum() != readedVarInfo->getTypeNum())
        {
            auto temp = createConvertedVariable(
                tgtInfo->getTypeNum(), readedVarInfo->getTypeNum(),
                readedVarInfo->getData()
            );
            tgtInfo->updateData(temp.get());
        }
        else
        {
            tgtInfo->updateData(readedVarInfo->getData());
        }
    }
    // we are still in the middle of a multi-dimentional array
    else
    {
        // if we are assigning to a temporary variable,
        // let it adopt the pointer directly
        if (pOpr->tgtName[0] == '#')
            getVarMgr().moveInArrayVariable(
                pOpr->tgtName, std::move(readedVarInfo)
            );
        // else, assign the pointer value
        else
        {
            auto tgtInfo = getVarMgr().getVariableInfo(pOpr->tgtName);
            auto temp = createConvertedVariable(
                tgtInfo->getTypeNum(), CARRAY, readedVarInfo->getData()
            );
            tgtInfo->updateData(temp.get());
        }
    }
}

void executionManager::exeWriteArrOpr(const writeArrayOperation *pOpr)
{
    auto rawInfo = getVarMgr().getVariableInfo(pOpr->arName);
    if (unlikely(rawInfo->getTypeNum() != CARRAY))
        throw badIntermediateCommand("executionManager::exeWriteArrOpr: "
                                     "read array operation is performing"
                                     " on a non-array variable");
    auto arInfo = static_cast<VariableInfoArray *>(rawInfo);
    auto intVec = prepareIndicies(pOpr->idxs);

    auto readedVarInfo = arInfo->address(intVec);

    // we get a single element from the array
    if (readedVarInfo->getTypeNum() != CARRAY)
    {
        auto srcInfo = getVarMgr().getVariableInfo(pOpr->srcName);
        if (srcInfo->getTypeNum() != readedVarInfo->getTypeNum())
        {
            auto temp = createConvertedVariable(
                readedVarInfo->getTypeNum(), srcInfo->getTypeNum(),
                srcInfo->getData()
            );
            readedVarInfo->updateData(temp.get());
        }
        else
        {
            readedVarInfo->updateData(srcInfo->getData());
        }
    }
    // we are still in the middle of a multi-dimentional array.
    // the write operation is invalid
    else
    {
        throw badIntermediateCommand("executionManager::exeWriteArrOpr: "
                                     "writing to an array variable");
    }
}

void executionManager::exeFuncCallOpr(const funcCallOperation *pOpr)
{
    // check if the function is a builtin one
    auto pBuiltin = getFuncMgr().getBuiltinInfo(pOpr->func);
    if (pBuiltin)
    {
        if (unlikely(pOpr->varVec.size() != pBuiltin->paramTypeNums.size()))
            throw funcArgumentError(pOpr->func);

        // prepare arguments
        std::vector<std::unique_ptr<uint8_t[]> > tmpVars;
        std::unique_ptr<const void *[]> pargs(
            new const void*[pOpr->varVec.size()]);
        for (int i = 0; i < pOpr->varVec.size(); ++i)
        {
            auto varType = getVarMgr().getVariableTypeNum(pOpr->varVec[i]);
            auto paramType = pBuiltin->paramTypeNums[i];
            if (varType == paramType)
            {
                pargs[i] = getVarMgr().getVariableInfo(pOpr->varVec[i])
                           ->getData();
            }
            else
            {
                tmpVars.emplace_back(createConvertedVariable(
                    paramType, varType,
                    getVarMgr().getVariableInfo(pOpr->varVec[i])->getData()
                ));
                pargs[i] = tmpVars.back().get();
            }
        }

        // invoke builtin function
        getFuncMgr().invokeBuiltin(pOpr->func, (const void **) pargs.get());

        return;
    }

    // push new cmds
    auto pFunc = getFuncMgr().getFunction(pOpr->func);
    nestedCmds.push_back(&pFunc->cmds);
    nestedCmdIdxs.push_back(0);

    // prepare arguments
    getVarMgr().newScope();
    if (unlikely(pOpr->varVec.size() != pFunc->paramNames.size()))
        throw funcArgumentError(pOpr->func);
    for (auto i = 0; i < pOpr->varVec.size(); ++i)
    {
        auto varType = getVarMgr().getVariableTypeNum(pOpr->varVec[i]);
        auto paramType = pFunc->paramTypeNums[i];

        // make type conversion if needed
        if (varType != paramType)
        {
            auto cvtedVar = createConvertedVariable(
                paramType, varType,
                getVarMgr().getVariableInfo(pOpr->varVec[i])->getData()
            );
            getVarMgr().initializeVariable(
                getTypeMgr().getTypenameByNum(paramType),
                pFunc->paramNames[i],
                cvtedVar.get(),
                true
            );
        }
        else
        {
            getVarMgr().initializeVariable(
                getTypeMgr().getTypenameByNum(paramType),
                pFunc->paramNames[i],
                getVarMgr().getVariableInfo(pOpr->varVec[i])->getData(),
                true
            );
        }
    }

    // set return variable type
    nestedRetTypes.push_back(pFunc->retType);

    // set function return value to void
    getVarMgr().setReturnValueToVoid();
}

void executionManager::exeFuncRetVoidOpr(const funcRetVoidOperation *pOpr)
{
    // search for the smallest function block
    ssize_t idx = nestedCmds.size() - 1;
    for (; idx >= 0; --idx)
        if (nestedCmds[idx]->type == cmdSeqType::function)
            break;
    if (unlikely(idx < 0))
        throw badIntermediateCommand(
            "executionManager::exeFuncRetVoidOpr: "
            "function return outside a function");

    // set return value to type void
    getVarMgr().setReturnValueToVoid();
    nestedRetTypes.pop_back();

    // if we are exiting the outermost function
    if (idx == 0)
    {
        for (int i = nestedCmds.size() - 1; i >= 0; --i)
            getVarMgr().popScope();
        nestedCmds.clear();
        nestedCmdIdxs.clear();
        return;
    }

    // update variable scope
    for (int i = nestedCmds.size() - 1; i >= idx; --i)
        getVarMgr().popScope();

    // jump back to previous function
    nestedCmds.resize(idx);
    nestedCmdIdxs.resize(idx);
}

void executionManager::exeFuncRetValOpr(const funcRetValOperation *pOpr)
{
    // search for the smallest function block
    ssize_t idx = nestedCmds.size() - 1;
    for (; idx >= 0; --idx)
        if (nestedCmds[idx]->type == cmdSeqType::function)
            break;
    if (unlikely(idx < 0))
        throw badIntermediateCommand(
            "executionManager::exeFuncRetValOpr: "
            "function return outside a function");

    // prepare return value
    auto pRetValType = getVarMgr().getVariableTypeNum(pOpr->vars[0]);
    auto pRetVal = getVarMgr().getVariableInfo(pOpr->vars[0])->getData();
    if (nestedRetTypes.back() != pRetValType)
    {
        auto temp = createConvertedVariable(nestedRetTypes.back(),
                                            pRetValType, pRetVal);
        getVarMgr().updateReturnValue(getTypeMgr().getTypenameByNum(
            nestedRetTypes.back()), temp.get());
    }
    else
    {
        getVarMgr().updateReturnValue(getTypeMgr().getTypenameByNum(
            pRetValType), pRetVal);
    }
    nestedRetTypes.pop_back();

    // if we are exiting the outermost function
    if (idx == 0)
    {
        for (int i = nestedCmds.size() - 1; i >= 0; --i)
            getVarMgr().popScope();
        nestedCmds.clear();
        nestedCmdIdxs.clear();
        return;
    }

    // update variable scope
    for (int i = nestedCmds.size() - 1; i >= idx; --i)
        getVarMgr().popScope();

    // jump back to previous function
    nestedCmds.resize(idx);
    nestedCmdIdxs.resize(idx);
}

void executionManager::exeLoopContOpr(const loopContOperation *pOpr)
{
    // search for the smallest loop block
    ssize_t idx = nestedCmds.size() - 1;
    for (; idx >= 0; --idx)
        if (nestedCmds[idx]->type == cmdSeqType::loop
            || unlikely(nestedCmds[idx]->type == cmdSeqType::function))
            break;
    if (unlikely(idx < 0))
        throw badIntermediateCommand(
            "executionManager::exeLoopContOpr: "
            "loop continue outside a loop");

    if (unlikely(idx == 0))
        throw badIntermediateCommand(
            "executionManager::exeLoopBrkOpr: "
            "loop is the out most block");

    // we are not in a loop block
    if (unlikely(nestedCmds[idx]->type == cmdSeqType::function))
        throw badIntermediateCommand(
            "executionManager::exeLoopContOpr: "
            "loop continue outside a loop");

    // update variable scope, we should have a brand new scope
    // after continuing the loop
    for (auto i = nestedCmds.size() - 1; i >= idx; --i)
        getVarMgr().popScope();
    getVarMgr().newScope();

    // jump back to the loop start
    nestedCmds.resize(idx + 1);
    nestedCmdIdxs.resize(idx + 1);
    nestedCmdIdxs.back() = 0;
}

void executionManager::exeLoopBrkOpr(const loopBrkOperation *pOpr)
{
    // search for the smallest loop block
    ssize_t idx = nestedCmds.size() - 1;
    for (; idx >= 0; --idx)
        if (nestedCmds[idx]->type == cmdSeqType::loop
            || unlikely(nestedCmds[idx]->type == cmdSeqType::function))
            break;
    if (unlikely(idx < 0))
        throw badIntermediateCommand(
            "executionManager::exeLoopBrkOpr: "
            "loop break outside a loop");
    if (unlikely(idx == 0))
        throw badIntermediateCommand(
            "executionManager::exeLoopBrkOpr: "
            "loop is the out most block");
    if (unlikely(nestedCmds[idx]->type == cmdSeqType::function))
        throw badIntermediateCommand(
            "executionManager::exeLoopBrkOpr: "
            "loop break outside a loop");

    // update variable scope
    for (auto i = nestedCmds.size() - 1; i >= idx; --i)
        getVarMgr().popScope();

    // quit loop
    nestedCmds.resize(idx);
    nestedCmdIdxs.resize(idx);
}

void executionManager::exeLoopGuardOpr(const loopGuardOperation *pOpr)
{
    // check currently we are in a loop block
    if (unlikely(nestedCmds.back()->type != cmdSeqType::loop))
        throw badIntermediateCommand(
            "executionManager::exeLoopGuardOpr: "
            "loop guard in non-loop block");

    // exit the loop if condition is false
    auto data = getVarMgr().getVariableInfo(pOpr->testVar)->getData();
    if (*reinterpret_cast<const int *>(data) == 0)
    {
        nestedCmds.pop_back();
        nestedCmdIdxs.pop_back();
        getVarMgr().popScope();
    }

    // otherwise, execute the loop body (do noting here)
}

void executionManager::exeLoopBlkOpr(const loopBlkOperation *pOpr)
{
    nestedCmds.push_back(&(pOpr->cmdseq));
    nestedCmdIdxs.push_back(0);
    getVarMgr().newScope();
}

void executionManager::exeBranchBlkOpr(const branchBlkOperation *pOpr)
{
    nestedCmds.push_back(&(pOpr->cmdseq));
    nestedCmdIdxs.push_back(0);
    getVarMgr().newScope();
}

void executionManager::exeBranchBrkOpr(const branchBrkOperation *pOpr)
{
    // search for the smallest branch block
    ssize_t idx = nestedCmds.size() - 1;
    for (; idx >= 0; --idx)
        if (nestedCmds[idx]->type == cmdSeqType::branch
            || unlikely(nestedCmds[idx]->type == cmdSeqType::function))
            break;
    if (unlikely(idx < 0))
        throw badIntermediateCommand(
            "executionManager::exeBranchBrkOpr: "
            "branch break outside a branch");
    if (unlikely(idx == 0))
        throw badIntermediateCommand(
            "executionManager::exeBranchBrkOpr: "
            "branch is the out most block");
    if (unlikely(nestedCmds[idx]->type == cmdSeqType::function))
        throw badIntermediateCommand(
            "executionManager::exeBranchBrkOpr: "
            "branch break outside a branch");

    // update variable scope
    for (auto i = nestedCmds.size() - 1; i >= idx; --i)
        getVarMgr().popScope();

    // quit branch
    nestedCmds.resize(idx);
    nestedCmdIdxs.resize(idx);
}

void executionManager::exeNormalBlkOpr(const normalBlkOperation *pOpr)
{
    nestedCmds.push_back(&(pOpr->cmdseq));
    nestedCmdIdxs.push_back(0);
    getVarMgr().newScope();
}

void executionManager::exeCondBlkOpr(const condBlkOperation *pOpr)
{
    // enter the conditional block if the condition holds true
    auto data = getVarMgr().getVariableInfo(pOpr->testVar)->getData();
    if (*reinterpret_cast<const int*>(data) != 0)
    {
        nestedCmds.push_back(&(pOpr->cmdseq));
        nestedCmdIdxs.push_back(0);
        getVarMgr().newScope();
    }
}

void executionManager::exeDeclVarOpr(const declVarOperation *pOpr)
{
    getVarMgr().declareVariable(pOpr->type, pOpr->var, false);
}

void executionManager::exeDeclArrOpr(const declArrOperation *pOpr)
{
    auto intVec = prepareIndicies(pOpr->idxs);
    getVarMgr().declareArrayVariable(
        pOpr->type, pOpr->var, intVec, false
    );
}

void executionManager::execute(const command &cmd)
{
    switch (cmd.type)
    {
    case cmdType::unaryOperation:
        exeUnaryOpr(reinterpret_cast<const unaryOperation*>(cmd.opr));
        break;
    case cmdType::binaryOperation:
        exeBinaryOpr(reinterpret_cast<const binaryOperation*>(cmd.opr));
        break;
    case cmdType::ternaryOperation:
        exeTernaryOpr(reinterpret_cast<const ternaryOperation*>(cmd.opr));
        break;
    case cmdType::readArrayOperation:
        exeReadArrOpr(reinterpret_cast<const readArrayOperation*>(cmd.opr));
        break;
    case cmdType::writeArrayOperation:
        exeWriteArrOpr(reinterpret_cast<const writeArrayOperation*>(cmd.opr));
        break;
    case cmdType::functionCall:
        exeFuncCallOpr(reinterpret_cast<const funcCallOperation*>(cmd.opr));
        break;
    case cmdType::functionReturnVoid:
        exeFuncRetVoidOpr(reinterpret_cast<const funcRetVoidOperation*>
                          (cmd.opr));
        break;
    case cmdType::functionReturnVal:
        exeFuncRetValOpr(reinterpret_cast<const funcRetValOperation*>
                          (cmd.opr));
        break;
    case cmdType::loopGuard:
        exeLoopGuardOpr(reinterpret_cast<const loopGuardOperation *>
            (cmd.opr));
        break;
    case cmdType::loopContinue:
        exeLoopContOpr(reinterpret_cast<const loopContOperation*>(cmd.opr));
        break;
    case cmdType::loopBreak:
        exeLoopBrkOpr(reinterpret_cast<const loopBrkOperation*>(cmd.opr));
        break;
    case cmdType::loopBlock:
        exeLoopBlkOpr(reinterpret_cast<const loopBlkOperation*>(cmd.opr));
        break;
    case cmdType::branchBlock:
        exeBranchBlkOpr(reinterpret_cast<const branchBlkOperation*>(cmd.opr));
        break;
    case cmdType::branchBreak:
        exeBranchBrkOpr(reinterpret_cast<const branchBrkOperation*>(cmd.opr));
        break;
    case cmdType::normalBlock:
        exeNormalBlkOpr(reinterpret_cast<const normalBlkOperation*>(cmd.opr));
        break;
    case cmdType::conditionalBlock:
        exeCondBlkOpr(reinterpret_cast<const condBlkOperation*>(cmd.opr));
        break;
    case cmdType::declareVariable:
        exeDeclVarOpr(reinterpret_cast<const declVarOperation*>(cmd.opr));
        break;
    case cmdType::declareArray:
        exeDeclArrOpr(reinterpret_cast<const declArrOperation*>(cmd.opr));
        break;
    default:
        throw unknownSwitchCase("executionManager::execute");
    }
}

void executionManager::run()
{
    while (nestedCmds.size())
    {
        // current cmd sequence is finished
        if (nestedCmdIdxs.back() >= nestedCmds.back()->cmds.size())
        {
            nestedCmds.pop_back();
            nestedCmdIdxs.pop_back();
            getVarMgr().popScope();
            continue;
        }

        // execute cmds
        ++nestedCmdIdxs.back();
        // printCmd(nestedCmds.back()->cmds[nestedCmdIdxs.back() - 1], 0);
        // std::cout.flush();
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
        execute(nestedCmds.back()->cmds[nestedCmdIdxs.back() - 1]);
    }
}

}  // namespace cint
