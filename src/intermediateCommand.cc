/********************************************************************
***********           Copyright @2019 Zhiyao Ma           ***********
*********************************************************************/

#include "intermediateCommand.hh"

namespace cint
{

void command::releaseMemory()
{
    switch (type)
    {
    case cmdType::unaryOperation:
        delete reinterpret_cast<unaryOprType *>(opr);
        break;
    case cmdType::binaryOperation:
        delete reinterpret_cast<binaryOprType *>(opr);
        break;
    case cmdType::ternaryOperation:
        delete reinterpret_cast<ternaryOprType *>(opr);
        break;
    case cmdType::readArrayOperation:
        delete reinterpret_cast<readArrayOperation *>(opr);
        break;
    case cmdType::writeArrayOperation:
        delete reinterpret_cast<writeArrayOperation *>(opr);
        break;
    case cmdType::functionCall:
        delete reinterpret_cast<funcCallOperation *>(opr);
        break;
    case cmdType::functionReturnVoid:
        delete reinterpret_cast<funcRetVoidOperation *>(opr);
        break;
    case cmdType::functionReturnVal:
        delete reinterpret_cast<funcRetValOperation *>(opr);
        break;
    case cmdType::loopContinue:
        delete reinterpret_cast<loopContOperation *>(opr);
        break;
    case cmdType::loopBreak:
        delete reinterpret_cast<loopBrkOperation *>(opr);
        break;
    case cmdType::loopGuard:
        delete reinterpret_cast<loopGuardOperation *>(opr);
        break;
    case cmdType::loopBlock:
        delete reinterpret_cast<loopBlkOperation *>(opr);
        break;
    case cmdType::branchBlock:
        delete reinterpret_cast<branchBlkOperation *>(opr);
        break;
    case cmdType::branchBreak:
        delete reinterpret_cast<branchBrkOperation *>(opr);
        break;
    case cmdType::normalBlock:
        delete reinterpret_cast<normalBlkOperation *>(opr);
        break;
    case cmdType::conditionalBlock:
        delete reinterpret_cast<condBlkOperation *>(opr);
        break;
    case cmdType::declareVariable:
        delete reinterpret_cast<declVarOperation *>(opr);
        break;
    case cmdType::declareArray:
        delete reinterpret_cast<declArrOperation *>(opr);
        break;
    case cmdType::empty:
        opr = nullptr;
        break;
    default:
        std::cerr << "Fatal error: unknown command type while destructing"
                  << " the command: " << this << std::endl;
        std::cerr << "cmdType number: " << static_cast<long>(type) << std::endl;
        std::terminate();
    }
}

command::~command()
{
    releaseMemory();
}

command::command(command &&other)
{
    type = other.type;
    opr = other.opr;
    other.type = cmdType::empty;
    other.opr = nullptr;
}

command& command::operator=(command &&other)
{
    releaseMemory();
    type = other.type;
    opr = other.opr;
    other.type = cmdType::empty;
    other.opr = nullptr;
    return *this;
}

}  // namespace cint
