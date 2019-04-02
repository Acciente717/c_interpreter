/********************************************************************
***********           Copyright @2019 Zhiyao Ma           ***********
*********************************************************************/

#include "intermediateCommand.hh"

namespace cint
{

command::~command()
{
    switch (type)
    {
    case cmdType::unaryOperation:
        unaryOprType *punary = reinterpret_cast<unaryOprType *>(opr);
        delete punary;
        break;
    case cmdType::binaryOperation:
        binaryOprType *pbinary = reinterpret_cast<binaryOprType *>(opr);
        delete pbinary;
        break;
    case cmdType::ternaryOperation:
        ternaryOprType *pternary = reinterpret_cast<ternaryOprType *>(opr);
        delete pternary;
        break;
    case cmdType::functionCall:
        funcCallOperation *pfcall = reinterpret_cast<funcCallOperation *>(opr);
        delete pfcall;
        break;
    case cmdType::functionReturn:
        funcRetOperation *pfret = reinterpret_cast<funcRetOperation *>(opr);
        delete pfret;
        break;
    case cmdType::loopContinue:
        loopContOperation *plcont = reinterpret_cast<loopContOperation *>(opr);
        delete plcont;
        break;
    case cmdType::loopBreak:
        loopBrkOperation *lbrk = reinterpret_cast<loopBrkOperation *>(opr);
        delete lbrk;
        break;
    case cmdType::branchBlock:
        branchBlkOperation *pbblk = reinterpret_cast<branchBlkOperation *>(opr);
        delete pbblk;
        break;
    case cmdType::branchBreak:
        branchBrkOperation *pbbrk = reinterpret_cast<branchBrkOperation *>(opr);
        delete pbbrk;
        break;
    case cmdType::normalBlock:
        normalBlkOperation *pnblk = reinterpret_cast<normalBlkOperation *>(opr);
        delete pnblk;
        break;
    case cmdType::conditionalBlock:
        condBlkOperation *pcblk = reinterpret_cast<condBlkOperation *>(opr);
        delete pcblk;
        break;
    default:
        std::cerr << "Fatal error: unknown command type while destructing"
                  << " the command" << std::endl;
        std::terminate();
    }
}

}  // namespace cint
