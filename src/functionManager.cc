/* Copyright @2019 Zhiyao Ma */

#include "typeManager.hh"
#include "functionManager.hh"
#include "builtinFunctions.hh"

namespace cint
{

functionManager &getFuncMgr()
{
    static functionManager mgr;
    return mgr;
}

void functionManager::initBuiltinFunctions()
{
    for (long i = 0; i < BUILTIN_FUNC_NUM; ++i)
    {
        switch (i)
        {
        case CPUTINT:
            builtins["cputint"] = {CPUTINT, {CLONG}};
            break;
        case CPUTCHAR:
            builtins["cputchar"] = {CPUTCHAR, {CLONG}};
            break;
        case CPUTDOUBLE:
            builtins["cputdouble"] = {CPUTDOUBLE, {CDOUBLE}};
            break;
        default:
            throw unknownFunction(
                "unknown builtin function number: " + std::to_string(i));
        }
    }
}

void functionManager::defineFunction(
    const std::string &name,
    std::vector<long> &&paramTypeNums,
    std::vector<long> &&paramBaseTypeNums,
    std::vector<std::vector<long>> &&paramSubscripts,
    std::vector<std::string> &&paramNames,
    long retType,
    bool isJITEnabled,
    int fileIdx,
    cmdSeq &&cmds)
{
    auto iter = funcs.find(name);
    if (iter != funcs.end())
        throw redefiningFunction(name);
    funcs.emplace(std::piecewise_construct,
                  std::forward_as_tuple(name),
                  std::forward_as_tuple(
                      std::move(paramTypeNums),
                      std::move(paramBaseTypeNums),
                      std::move(paramSubscripts),
                      std::move(paramNames),
                      retType,
                      isJITEnabled,
                      fileIdx,
                      std::move(cmds)));
}

functionInfo * functionManager::getFunction(const std::string &name)
{
    auto iter = funcs.find(name);
    if (iter == funcs.end())
        throw unknownFunction(name);
    return &iter->second;
}

const builtinFuncInfo * functionManager::getBuiltinInfo(
    const std::string &funcName)
{
    auto iter = builtins.find(funcName);
    if (iter == builtins.end())
        return nullptr;
    return &iter->second;
}

void functionManager::invokeBuiltin(const std::string &funcName,
                                    const void *pparams[])
{
    auto iter = builtins.find(funcName);
    if (unlikely(iter == builtins.end()))
        throw unknownFunction("unknown builtin function: " + funcName);

    switch (iter->second.builtinNum)
    {
    case CPUTINT:
        cputint(*reinterpret_cast<const long*>(pparams[0]));
        break;
    case CPUTCHAR:
        cputchar(*reinterpret_cast<const long*>(pparams[0]));
        break;
    case CPUTDOUBLE:
        cputdouble(*reinterpret_cast<const double*>(pparams[0]));
        break;
    default:
        throw unknownFunction(
            "unknown builtin function number: "
            + std::to_string(iter->second.builtinNum));
    }
}

void functionManager::compileFunction(functionInfo *pFunc)
{
    std::string currentPath = gArgv[0];
    auto found = currentPath.find_last_of("/");
    std::string currentDir;
    if (found != std::string::npos)
        currentDir = currentPath.substr(0, found + 1);
    else
        currentDir = "./";
    std::string fileName = gArgv[pFunc->fileIdx];
    auto systemCmd = currentDir + "jit.py " + fileName;
    pFunc->status = COMPILING;
    system(systemCmd.c_str());
    pFunc->status = COMPILED;
}

}  // namespace cint
