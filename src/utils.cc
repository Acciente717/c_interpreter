/* Copyright @2019 Zhiyao Ma */

#include <string>

#include "utils.hh"
#include "intermediateCommand.hh"

namespace cint
{

std::string genRandomStr(const size_t len, const bool sharpHead)
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    iferr (len == 0)
        return std::string();

    int i;
    std::string str(len, 0);
    if (sharpHead)
        str[0] = '#', i = 1;
    else
        i = 0;

    for (; i < len; ++i)
    {
        str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return str;
}

void printCmdSeq(const cmdSeq &seq, int indent)
{
    using cint::cmdType;
    using cint::ternaryOprType;
    using std::cout;
    using std::endl;

    std::string spaces(indent * 4, ' ');

    for (auto &i : seq.cmds)
    {
        cout << spaces;
        switch (i.type)
        {
        case cmdType::binaryOperation:
            cout << "binaryOperation: ";
            switch (reinterpret_cast<binaryOperation *>(i.opr)->oprType)
            {
                case binaryOprType::assign:
                    cout << "assign: ";
                    break;
                default:
                    cout << "unknown: ";
                    break;
            }
            cout << reinterpret_cast<binaryOperation *>(i.opr)->vars[0] << " | "
                 << reinterpret_cast<binaryOperation *>(i.opr)->vars[1] << endl;
            break;
        // end of `binaryOperation`

        case cmdType::ternaryOperation:
            cout << "ternaryOperation: ";
            switch (reinterpret_cast<ternaryOperation *>(i.opr)->oprType)
            {
            case ternaryOprType::assignSum:
                cout << "assignSum: ";
                break;
            case ternaryOprType::assignDifference:
                cout << "assignDifference: ";
                break;
            case ternaryOprType::assignProduct:
                cout << "assignProduct: ";
                break;
            case ternaryOprType::assignQuotient:
                cout << "assignQuotient: ";
                break;
            case ternaryOprType::assignResidue:
                cout << "assignResidue: ";
                break;
            case ternaryOprType::assignLess:
                cout << "assignLess: ";
                break;
            case ternaryOprType::assignLessEqual:
                cout << "assignLessEqual: ";
                break;
            case ternaryOprType::assignGreater:
                cout << "assignGreater: ";
                break;
            case ternaryOprType::assignGreaterEqual:
                cout << "assignGreaterEqual: ";
                break;
            case ternaryOprType::assignEqual:
                cout << "assignEqual: ";
                break;
            case ternaryOprType::assignNotEqual:
                cout << "assignNotEqual: ";
                break;
            case ternaryOprType::assignLogicAnd:
                cout << "assignLogicAnd: ";
                break;
            case ternaryOprType::assignLogicOr:
                cout << "assignLogicOr: ";
                break;
            default:
                cout << "unknown: ";
                break;
            }
            cout << reinterpret_cast<ternaryOperation *>(i.opr)->vars[0]
                 << " | "
                 << reinterpret_cast<ternaryOperation *>(i.opr)->vars[1]
                 << " | "
                 << reinterpret_cast<ternaryOperation *>(i.opr)->vars[2]
                 << endl;
            break;
        // end of `ternaryOperation`

        case cmdType::normalBlock:
            cout << "normalBlock: " << endl;
            printCmdSeq(reinterpret_cast<normalBlkOperation *>(i.opr)->cmdseq,
                        indent + 1);
            break;
        // end of `normalBlock`

        case cmdType::branchBreak:
            cout << "branchBreak !" << endl;
            break;
        // end of `branchBreak`

        case cmdType::branchBlock:
            cout << "branchBlock: " << endl;
            printCmdSeq(reinterpret_cast<branchBlkOperation *>(i.opr)->cmdseq,
                        indent + 1);
            break;
        // end of `branchBlock`

        case cmdType::conditionalBlock:
            cout << "conditionalBlock ("
                 << reinterpret_cast<condBlkOperation *>(i.opr)->testVar
                 << "): " << endl;
            printCmdSeq(reinterpret_cast<condBlkOperation *>(i.opr)->cmdseq,
                        indent + 1);
            break;
        // end of `conditionalBlock`

        case cmdType::loopBlock:
            cout << "loopBlock: " << endl;
            printCmdSeq(reinterpret_cast<loopBlkOperation *>(i.opr)->cmdseq,
                        indent + 1);
            break;
        // end of `loopBlock`

        case cmdType::loopGuard:
            cout << "loopGuard ("
                 << reinterpret_cast<loopGuardOperation *>(i.opr)->testVar
                 << ")" << endl;
            break;
        // end of `loopGuard`

        case cmdType::loopContinue:
            cout << "loopContinue !" << endl;
            break;
        // end of `loopContinue`

        case cmdType::loopBreak:
            cout << "loopBreak !" << endl;
            break;
        // end of `loopBreak`

        default:
            cout << "unknown: " << endl;
        }
    }
}

}  // namespace cint
