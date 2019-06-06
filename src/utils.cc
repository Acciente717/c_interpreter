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

void printCmd(const command &cmd, int indent)
{
    using cint::cmdType;
    using cint::ternaryOprType;
    using std::cout;
    using std::endl;

    std::string spaces(indent * 4, ' ');
    cout << spaces;

    switch (cmd.type)
    {
    case cmdType::unaryOperation:
        cout << "unaryOperation: ";
        switch (reinterpret_cast<unaryOperation *>(cmd.opr)->oprType)
        {
        case unaryOprType::getReturnValue:
            cout << "getReturnValue: ";
            break;
        default:
            cout << "unknown: ";
            break;
        }
        cout << reinterpret_cast<unaryOperation *>(cmd.opr)->vars[0] << endl;
        break;
    case cmdType::binaryOperation:
        cout << "binaryOperation: ";
        switch (reinterpret_cast<binaryOperation *>(cmd.opr)->oprType)
        {
        case binaryOprType::assignVariable:
            cout << "assignVariable: ";
            break;
        case binaryOprType::assignLiteral:
            cout << "assignLiteral: ";
            break;
        case binaryOprType::assignNot:
            cout << "assignNot: ";
            break;
        case binaryOprType::assignNegate:
            cout << "assignNegate: ";
            break;
        default:
            cout << "unknown: ";
            break;
        }
        cout << reinterpret_cast<binaryOperation *>(cmd.opr)->vars[0] << " | "
             << reinterpret_cast<binaryOperation *>(cmd.opr)->vars[1] << endl;
        break;
        // end of `binaryOperation`

    case cmdType::ternaryOperation:
        cout << "ternaryOperation: ";
        switch (reinterpret_cast<ternaryOperation *>(cmd.opr)->oprType)
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
        cout << reinterpret_cast<ternaryOperation *>(cmd.opr)->vars[0]
             << " | "
             << reinterpret_cast<ternaryOperation *>(cmd.opr)->vars[1]
             << " | "
             << reinterpret_cast<ternaryOperation *>(cmd.opr)->vars[2]
             << endl;
        break;
        // end of `ternaryOperation`

    case cmdType::normalBlock:
        cout << "normalBlock: " << endl;
        printCmdSeq(reinterpret_cast<normalBlkOperation *>(cmd.opr)->cmdseq,
                    indent + 1);
        break;
        // end of `normalBlock`

    case cmdType::branchBreak:
        cout << "branchBreak !" << endl;
        break;
        // end of `branchBreak`

    case cmdType::branchBlock:
        cout << "branchBlock: " << endl;
        printCmdSeq(reinterpret_cast<branchBlkOperation *>(cmd.opr)->cmdseq,
                    indent + 1);
        break;
        // end of `branchBlock`

    case cmdType::conditionalBlock:
        cout << "conditionalBlock ("
             << reinterpret_cast<condBlkOperation *>(cmd.opr)->testVar
             << "): " << endl;
        printCmdSeq(reinterpret_cast<condBlkOperation *>(cmd.opr)->cmdseq,
                    indent + 1);
        break;
        // end of `conditionalBlock`

    case cmdType::loopBlock:
        cout << "loopBlock: " << endl;
        printCmdSeq(reinterpret_cast<loopBlkOperation *>(cmd.opr)->cmdseq,
                    indent + 1);
        break;
        // end of `loopBlock`

    case cmdType::loopGuard:
        cout << "loopGuard ("
             << reinterpret_cast<loopGuardOperation *>(cmd.opr)->testVar
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

    case cmdType::readArrayOperation:
        cout << "readArray: "
             << reinterpret_cast<readArrayOperation *>(cmd.opr)->tgtName
             << " | "
             << reinterpret_cast<readArrayOperation *>(cmd.opr)->arName;
        for (const auto &j :
             reinterpret_cast<readArrayOperation *>(cmd.opr)->idxs)
            cout << '[' << j << ']';
        cout << endl;
        break;
        // end of `readArrayOperation`

    case cmdType::writeArrayOperation:
        cout << "writeArray: "
             << reinterpret_cast<writeArrayOperation *>(cmd.opr)->arName;
        for (const auto &j :
             reinterpret_cast<writeArrayOperation *>(cmd.opr)->idxs)
            cout << '[' << j << ']';
        cout << " | "
             << reinterpret_cast<writeArrayOperation *>(cmd.opr)->srcName
             << endl;
        break;
        // end of `writeArrayOperation`

    case cmdType::functionCall:
        cout << "functionCall: "
             << reinterpret_cast<funcCallOperation *>(cmd.opr)->func
             << '(';
        for (int j = 0;
             j < reinterpret_cast<funcCallOperation *>(cmd.opr)->varVec.size();
             ++j)
        {
            if (j != 0)
                cout << ", ";
            cout << reinterpret_cast<funcCallOperation *>(cmd.opr)->varVec[j];
        }
        cout << ')' << endl;
        break;
        // end of `funcCallOperation`

    case cmdType::functionReturnVoid:
        cout << "functionReturnVoid !" << endl;
        break;
        // end of `functionReturnVoid`

    case cmdType::functionReturnVal:
        cout << "functionReturnVal: "
             << reinterpret_cast<funcRetValOperation *>(cmd.opr)->vars[0]
             << " !" << endl;
        break;
        // end of `functionReturnVal`

    case cmdType::declareVariable:
        cout << "declareVarable: ["
             << reinterpret_cast<declVarOperation *>(cmd.opr)->type
             << "] "
             << reinterpret_cast<declVarOperation *>(cmd.opr)->var
             << endl;
        break;
        // end of `declareVariable`

    case cmdType::declareArray:
        cout << "declareArray: ["
             << reinterpret_cast<declArrOperation *>(cmd.opr)->type
             << "] "
             << reinterpret_cast<declArrOperation *>(cmd.opr)->var;
        for (const auto &j :
                reinterpret_cast<declArrOperation *>(cmd.opr)->idxs)
            cout << '[' << j << ']';
        cout << endl;
        break;
        // end of `declareArray`

    default:
        cout << "unknown: " << endl;
    }
}

void printCmdSeq(const cmdSeq &seq, int indent)
{
    for (auto &i : seq.cmds)
        printCmd(i, indent);
}

}  // namespace cint
