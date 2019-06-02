/* Copyright @2019 Zhiyao Ma */

#include "inc/functionManager.hh"

namespace cint
{

functionManager &getFuncMgr()
{
    static functionManager mgr;
    return mgr;
}

void functionManager::defineFunction(
    const std::string &name,
    const std::vector<int> &paramTypeNums,
    const std::vector<std::string> &paramNames,
    const cmdSeq &cmds)
{
    auto iter = funcs.find(name);
    if (iter != funcs.end())
        throw redefiningFunction(name);
    funcs[name] = {paramTypeNums, paramNames, cmds};
}

const functionInfo * functionManager::getFunction(const std::string &name)
{
    auto iter = funcs.find(name);
    if (iter == funcs.end())
        throw unknownFunction(name);
    return &funcs[name];
}

}  // namespace cint
