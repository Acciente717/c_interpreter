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
    for (int i = 0; i < BUILTIN_FUNC_NUM; ++i)
    {
        switch (i)
        {
        case CPUTINT:
            builtins["cputint"] = {CPUTINT, {CINT32}};
            break;
        case CPUTCHAR:
            builtins["cputchar"] = {CPUTCHAR, {CINT32}};
            break;
        default:
            throw unknownFunction(
                "unknown builtin function number: " + std::to_string(i));
        }
    }
}

void functionManager::defineFunction(
    const std::string &name,
    std::vector<int> &&paramTypeNums,
    std::vector<std::string> &&paramNames,
    cmdSeq &&cmds)
{
    auto iter = funcs.find(name);
    if (iter != funcs.end())
        throw redefiningFunction(name);
    funcs[name] = {std::move(paramTypeNums),
                   std::move(paramNames),
                   std::move(cmds)};
}

const functionInfo * functionManager::getFunction(const std::string &name)
{
    auto iter = funcs.find(name);
    if (iter == funcs.end())
        throw unknownFunction(name);
    return &funcs[name];
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
        cputint(*reinterpret_cast<const int*>(pparams[0]));
        break;
    case CPUTCHAR:
        cputchar(*reinterpret_cast<const int*>(pparams[0]));
        break;
    default:
        throw unknownFunction(
            "unknown builtin function number: "
            + std::to_string(iter->second.builtinNum));
    }
}

}  // namespace cint
