/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_EXECUTION_MANAGER_HH_
#define SRC_INC_EXECUTION_MANAGER_HH_

#include "intermediateCommand.hh"
#include "variableManager.hh"

namespace cint
{

class executionManager
{
    std::vector<const cmdSeq *> nestedCmds;
    std::vector<int> nestedCmdIdxs;

    void exeUnaryOpr(const unaryOperation *pOpr);
    void exeBinaryOpr(const binaryOperation *pOpr);
    void exeTernaryOpr(const ternaryOperation *pOpr);
    void exeReadArrOpr(const readArrayOperation *pOpr);
    void exeWriteArrOpr(const writeArrayOperation *pOpr);
    void exeFuncCallOpr(const funcCallOperation *pOpr);
    void exeFuncRetOpr(const funcRetOperation *pOpr);
    void exeLoopContOpr(const loopContOperation *pOpr);
    void exeLoopBrkOpr(const loopBrkOperation *pOpr);
    void exeLoopGuardOpr(const loopGuardOperation *pOpr);
    void exeLoopBlkOpr(const loopBlkOperation *pOpr);
    void exeBranchBlkOpr(const branchBlkOperation *pOpr);
    void exeBranchBrkOpr(const branchBrkOperation *pOpr);
    void exeNormalBlkOpr(const normalBlkOperation *pOpr);
    void exeCondBlkOpr(const condBlkOperation *pOpr);
    void exeDeclVarOpr(const declVarOperation *pOpr);
    void execute(const command &cmd);
 public:
    inline explicit executionManager(const cmdSeq *init_cmds);
    void run();
};

inline executionManager::executionManager(const cmdSeq *init_cmds)
{
    nestedCmds.push_back(init_cmds);
    nestedCmdIdxs.push_back(0);
}


}  // namespace cint

#endif
