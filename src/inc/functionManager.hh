/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_FUNCTION_MANAGER_HH_
#define SRC_INC_FUNCTION_MANAGER_HH_

#include <string>
#include <vector>
#include <unordered_map>

#include "intermediateCommand.hh"

namespace cint
{

struct functionInfo
{
    std::vector<int> paramTypeNums;
    std::vector<std::string> paramNames;
    cmdSeq cmds;
};

class functionManager
{
    std::unordered_map<std::string, functionInfo> funcs;
 public:
    void defineFunction(const std::string &name,
                        const std::vector<int> &paramTypeNums,
                        const std::vector<std::string> &paramNames,
                        const cmdSeq &cmds);
    const functionInfo * getFunction(const std::string &name);
};

functionManager &getFuncMgr();

}  // namespace cint

#endif  // SRC_INC_FUNCTION_MANAGER_HH_
