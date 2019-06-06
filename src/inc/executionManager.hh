/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_EXECUTION_MANAGER_HH_
#define SRC_INC_EXECUTION_MANAGER_HH_

#include "intermediateCommand.hh"
#include "variableManager.hh"
#include "functionManager.hh"

namespace cint
{

class executionManager
{
    std::vector<const cmdSeq *> nestedCmds;
    std::vector<int> nestedCmdIdxs;
    std::vector<int> nestedRetTypes;

    void exeUnaryOpr(const unaryOperation *pOpr);
    void exeBinaryOpr(const binaryOperation *pOpr);
    void exeTernaryOpr(const ternaryOperation *pOpr);
    void exeReadArrOpr(const readArrayOperation *pOpr);
    void exeWriteArrOpr(const writeArrayOperation *pOpr);
    void exeFuncCallOpr(const funcCallOperation *pOpr);
    void exeFuncRetVoidOpr(const funcRetVoidOperation *pOpr);
    void exeFuncRetValOpr(const funcRetValOperation *pOpr);
    void exeLoopContOpr(const loopContOperation *pOpr);
    void exeLoopBrkOpr(const loopBrkOperation *pOpr);
    void exeLoopGuardOpr(const loopGuardOperation *pOpr);
    void exeLoopBlkOpr(const loopBlkOperation *pOpr);
    void exeBranchBlkOpr(const branchBlkOperation *pOpr);
    void exeBranchBrkOpr(const branchBrkOperation *pOpr);
    void exeNormalBlkOpr(const normalBlkOperation *pOpr);
    void exeCondBlkOpr(const condBlkOperation *pOpr);
    void exeDeclVarOpr(const declVarOperation *pOpr);
    void exeDeclArrOpr(const declArrOperation *pOpr);
    void execute(const command &cmd);
 public:
    inline explicit executionManager(const std::string &entryFunc);
    void run();
};

inline executionManager::executionManager(const std::string &entryFunc)
{
    auto entry = getFuncMgr().getFunction(entryFunc);
    nestedCmds.push_back(&entry->cmds);
    nestedCmdIdxs.push_back(0);
    nestedRetTypes.push_back(entry->retType);
}


}  // namespace cint

#endif
