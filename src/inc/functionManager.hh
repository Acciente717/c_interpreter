/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_FUNCTION_MANAGER_HH_
#define SRC_INC_FUNCTION_MANAGER_HH_

#include <string>
#include <vector>
#include <unordered_map>
#include <atomic>
#include <future>

#include "intermediateCommand.hh"

extern int gArgc;         //  global access for argc
extern char **gArgv;      //  global access for argv
extern int gReadArgc;     //  the number of processed files

namespace cint
{

enum builtinFuncEnum
{
    CPUTINT,
    CPUTCHAR,
    CPUTDOUBLE,
    BUILTIN_FUNC_NUM
};

enum functionStatus
{
    NO_JIT,
    NOT_COMPILED,
    COMPILING,
    COMPILED,
    JIT_AVAILABLE,
    JIT_DIRECT
};

struct builtinFuncInfo
{
    builtinFuncEnum builtinNum;
    std::vector<long> paramTypeNums;
};

struct functionInfo
{
    std::vector<long> paramTypeNums;
    std::vector<long> paramBaseTypeNums;
    std::vector<std::vector<long>> paramSubscripts;
    std::vector<std::string> paramNames;
    long retType;
    std::atomic_int status;
    void *compiledEntrance;
    std::future<void> fut;
    int fileIdx;
    cmdSeq cmds;

    functionInfo(std::vector<long> &&_paramTypeNums,
                 std::vector<long> &&_paramBaseTypeNums,
                 std::vector<std::vector<long>> &&_paramSubscripts,
                 std::vector<std::string> &&_paramNames,
                 long _retType, bool _isJITable, int _fileIdx,
                 cmdSeq &&_cmds)
    : paramTypeNums(std::move(_paramTypeNums)),
      paramBaseTypeNums(std::move(_paramBaseTypeNums)),
      paramSubscripts(std::move(_paramSubscripts)),
      paramNames(std::move(_paramNames)),
      retType(_retType), status(_isJITable ? NOT_COMPILED : NO_JIT),
      compiledEntrance(0), fileIdx(_fileIdx), cmds(std::move(_cmds)) {}
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
                        std::vector<long> &&paramBaseTypeNums,
                        std::vector<std::vector<long>> &&paramSubscripts,
                        std::vector<std::string> &&paramNames,
                        long retType,
                        bool JITEnabled,
                        int fileIdx,
                        cmdSeq &&cmds);
    inline const decltype(funcs) *getAllDefinedFuncs();
    functionInfo * getFunction(const std::string &name);
    const builtinFuncInfo * getBuiltinInfo(const std::string &funcName);
    void invokeBuiltin(const std::string &funcName, const void *pparams[]);
    static void compileFunction(functionInfo *pFunc);
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
