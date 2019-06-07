/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_FUNCTION_MANAGER_HH_
#define SRC_INC_FUNCTION_MANAGER_HH_

#include <string>
#include <vector>
#include <unordered_map>

#include "intermediateCommand.hh"

namespace cint
{

enum builtinFuncEnum
{
    CPUTINT,
    CPUTCHAR,
    CPUTDOUBLE,
    BUILTIN_FUNC_NUM
};

struct builtinFuncInfo
{
    builtinFuncEnum builtinNum;
    std::vector<long> paramTypeNums;
};

struct functionInfo
{
    std::vector<long> paramTypeNums;
    std::vector<std::string> paramNames;
    long retType;
    cmdSeq cmds;
};

class functionManager
{
    std::unordered_map<std::string, functionInfo> funcs;  // user defined
    std::unordered_map<std::string, builtinFuncInfo> builtins;  // builtin

    void initBuiltinFunctions();
 public:
    inline functionManager();

    void defineFunction(const std::string &name,
                        std::vector<long> &&paramTypeNums,
                        std::vector<std::string> &&paramNames,
                        long retType,
                        cmdSeq &&cmds);
    inline const decltype(funcs) *getAllDefinedFuncs();
    const functionInfo * getFunction(const std::string &name);
    const builtinFuncInfo * getBuiltinInfo(const std::string &funcName);
    void invokeBuiltin(const std::string &funcName, const void *pparams[]);
};

functionManager &getFuncMgr();

inline functionManager::functionManager()
{
    initBuiltinFunctions();
}


inline const decltype(functionManager::funcs)
*functionManager::getAllDefinedFuncs()
{
    return &funcs;
}

}  // namespace cint

#endif  // SRC_INC_FUNCTION_MANAGER_HH_
