/* Copyright @2019 Zhiyao Ma */
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
            *reinterpret_cast<float*>(new_data.get())
                = *reinterpret_cast<const int32_t*>(data);
            break;
        }
        break;
    case CFLOAT:
        switch (srcTypeNum)
        {
        case CINT32:
            *reinterpret_cast<int32_t*>(new_data.get())
                = *reinterpret_cast<const float*>(data);
            break;
        case CFLOAT:
            *reinterpret_cast<float*>(new_data.get())
                = *reinterpret_cast<const float*>(data);
            break;
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
    auto *pIntX = reinterpret_cast<float*>(px);
    auto *pIntY = reinterpret_cast<const float*>(py);
    auto *pIntZ = reinterpret_cast<const float*>(pz);

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
        throw unsupportedArithmic("cannot apply moduolo to floats");
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
        throw unknownSwitchCase("floatArithmic");
    }
}

void executionManager::exeUnaryOpr(const unaryOperation *pOpr)
{
    // currently no unary operation
}

void executionManager::exeBinaryOpr(const binaryOperation *pOpr)
{
    int xType, yType;
    void *xPtr, *yPtr;

    switch (pOpr->oprType)
    {
    case binaryOprType::assignVariable:
        xType = getVarMgr().getVariableTypeNum(pOpr->vars[0]);
        yType = getVarMgr().getVariableTypeNum(pOpr->vars[1]);
        xPtr = getVarMgr().getVariableData(pOpr->vars[0]);
        yPtr = getVarMgr().getVariableData(pOpr->vars[1]);
        if (xType != yType)
        {
            auto temp = createConvertedVariable(xType, yType, yPtr);
            memcpy(xPtr, temp.get(), basicTypesSize[xType]);
        }
        else
        {
            memcpy(xPtr, yPtr, basicTypesSize[xType]);
        }
        break;
    case binaryOprType::assignLiteral:
        xType = getVarMgr().getVariableTypeNum(pOpr->vars[0]);
        xPtr = getVarMgr().getVariableData(pOpr->vars[0]);
        switch (xType)
        {
        case CINT32:
            {
                auto value = std::stoi(pOpr->vars[1]);
                memcpy(xPtr, &value, basicTypesSize[CINT32]);
            }
            break;
        case CFLOAT:
            {
                auto value = std::stof(pOpr->vars[1]);
                memcpy(xPtr, &value, basicTypesSize[CFLOAT]);
            }
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
    auto yType = getVarMgr().getVariableTypeNum(pOpr->vars[1]);
    auto zType = getVarMgr().getVariableTypeNum(pOpr->vars[2]);
    auto yPtr = getVarMgr().getVariableData(pOpr->vars[1]);
    auto zPtr = getVarMgr().getVariableData(pOpr->vars[2]);
    void *tgtPtr;

    std::unique_ptr<uint8_t[]> xSmart, ySmart, zSmart;
    int superType;

    // if y and z have different types, convert them before proceeding
    if (yType != zType)
    {
        superType = typeManager::chooseSuperType(yType, zType);
        if (yType != superType)
        {
            ySmart = createConvertedVariable(superType, yType, yPtr);
            yPtr = ySmart.get();
        }
        if (zType != superType)
        {
            zSmart = createConvertedVariable(superType, zType, zPtr);
            zPtr = zSmart.get();
        }
    }

    // If x is a temporary variable, we must first declare it to the
    // variable manager.
    if (pOpr->vars[0][0] == '#')
    {
        getVarMgr().declareVariable(
            getTypeMgr().getTypenameByNum(superType),
            pOpr->vars[0]);
    }

    auto xType = getVarMgr().getVariableTypeNum(pOpr->vars[0]);
    auto xPtr = getVarMgr().getVariableData(pOpr->vars[0]);

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
        if (xType != superType)
        {
            xSmart.reset(new uint8_t[basicTypesSize[superType]]);
            tgtPtr = xSmart.get();
        }
        else
        {
            tgtPtr = xPtr;
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
        if (xType != CINT32)
        {
            xSmart.reset(new uint8_t[basicTypesSize[CINT32]]);
            tgtPtr = xSmart.get();
        }
        else
        {
            tgtPtr = xPtr;
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
    if (xType != superType)
    {
        xSmart = createConvertedVariable(xType, superType, xSmart.get());
        memcpy(xPtr, xSmart.get(), basicTypesSize[xType]);
    }
}

void executionManager::exeReadArrOpr(const readArrayOperation *pOpr)
{
    throw notImplemented("executionManager::exeReadArrOpr");
}

void executionManager::exeWriteArrOpr(const writeArrayOperation *pOpr)
{
    throw notImplemented("executionManager::exeWriteArrOpr");
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
        std::unique_ptr<void *[]> pargs(new void*[pOpr->varVec.size()]);
        for (int i = 0; i < pOpr->varVec.size(); ++i)
        {
            auto varType = getVarMgr().getVariableTypeNum(pOpr->varVec[i]);
            auto paramType = pBuiltin->paramTypeNums[i];
            if (varType == paramType)
            {
                pargs[i] = getVarMgr().getVariableData(pOpr->varVec[i]);
            }
            else
            {
                tmpVars.emplace_back(createConvertedVariable(
                    paramType, varType,
                    getVarMgr().getVariableData(pOpr->varVec[i])
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
                getVarMgr().getVariableData(pOpr->varVec[i])
            );
            getVarMgr().initializeVariable(
                getTypeMgr().getTypenameByNum(paramType),
                pFunc->paramNames[i],
                cvtedVar.get()
            );
        }
        else
        {
            getVarMgr().initializeVariable(
                getTypeMgr().getTypenameByNum(paramType),
                pFunc->paramNames[i],
                getVarMgr().getVariableData(pOpr->varVec[i])
            );
        }
    }
}

void executionManager::exeFuncRetOpr(const funcRetOperation *pOpr)
{
    // search for the smallest function block
    ssize_t idx = nestedCmds.size() - 1;
    for (; idx >= 0; --idx)
        if (nestedCmds[idx]->type == cmdSeqType::function)
            break;
    if (unlikely(idx < 0))
        throw badIntermediateCommand(
            "executionManager::exeFuncRetOpr: "
            "function return outside a function");

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
    auto data = getVarMgr().getVariableData(pOpr->testVar);
    if (*reinterpret_cast<int *>(data) == 0)
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
    auto data = getVarMgr().getVariableData(pOpr->testVar);
    if (*reinterpret_cast<int*>(data) != 0)
    {
        nestedCmds.push_back(&(pOpr->cmdseq));
        nestedCmdIdxs.push_back(0);
        getVarMgr().newScope();
    }
}

void executionManager::exeDeclVarOpr(const declVarOperation *pOpr)
{
    getVarMgr().declareVariable(pOpr->type, pOpr->var);
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
    case cmdType::functionReturn:
        exeFuncRetOpr(reinterpret_cast<const funcRetOperation*>(cmd.opr));
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
        execute(nestedCmds.back()->cmds[nestedCmdIdxs.back() - 1]);
    }
}

}  // namespace cint
