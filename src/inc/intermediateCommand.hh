/* Copyright @2019 Zhiyao Ma */
#ifndef SRC_INC_INTERMEDIATE_COMMAND_HH_
#define SRC_INC_INTERMEDIATE_COMMAND_HH_

#include <string>
#include <memory>
#include <vector>
#include <iostream>

#include "exceptions.hh"
#include "representations.hh"

namespace cint
{

/*******************************************************************
 *                    Alias and Declarations                       *
 ******************************************************************/

using VarName = std::string;
using FuncName = std::string;
using TypeName = std::string;

struct command;

/*******************************************************************
 *                   Definition of Structures                      *
 ******************************************************************/

/**
 *  Types of all command sequences.
 * 
 *  function - commands in a function
 *  loop     - commands in a loop block
 *  branch   - commands when testing a sequence of judgement
 *  normal   - commands enclosed by a pair of `{ ... }`
 */
enum class cmdSeqType
{
    function,
    loop,
    branch,
    normal
};

/**
 *  A sequence of commands.
 */
struct cmdSeq
{
    cmdSeqType type;
    std::vector<command> cmds;
};

/**
 *  Types of all possible intermediate commands.
 */
enum class cmdType
{
    empty,
    unaryOperation,
    binaryOperation,
    ternaryOperation,
    readArrayOperation,
    writeArrayOperation,
    functionCall,
    functionReturnVoid,
    functionReturnVal,
    loopContinue,
    loopBreak,
    loopGuard,
    loopBlock,
    branchBreak,
    branchBlock,
    normalBlock,
    conditionalBlock,
    declareVariable
};

/**
 *  Types of all possible unary operations.
 */
enum class unaryOprType
{
    getReturnValue
};

/**
 *  Types of all possible binary operations.
 * 
 *  Operation: f(x, y)
 *  assignVariable   - x = y
 *  assignLiteral    - x = y
 *  assignNot        - x = !y
 *  assignNegate     - x = -y
 */
enum class binaryOprType
{
    assignVariable,
    assignLiteral,
    assignNot,
    assignNegate
};

/**
 *  WARNING: Execution Manager use this sequence to perform
 *  type conversion decision. Modify it with caution!
 * 
 *  Types of all possible ternary operations.
 * 
 *  Operation: f(x, y, z)
 *  assignSum          - x = y + z
 *  assignDifference   - x = y - z
 *  assignProduct      - x = y * z
 *  assignQuotient     - x = y / z
 *  assignResidue      - x = y % z
 *  assignLess         - x = y < z
 *  assignLessEqual    - x = y <= z
 *  assignGreater      - x = y > z
 *  assignGreaterEqual - x = y >= z
 *  assignEqual        - x = y == z
 *  assignNotEqual     - x = y != z
 *  assignLogicAnd     - x = y && z
 *  assignLogicOr      - x = y || z
 */
enum class ternaryOprType
{
    assignSum,
    assignDifference,
    assignProduct,
    assignQuotient,
    assignResidue,
    assignLess,
    assignLessEqual,
    assignGreater,
    assignGreaterEqual,
    assignEqual,
    assignNotEqual,
    assignLogicAnd,
    assignLogicOr
};

/**
 *  Unary operation.
 * 
 *  Operation: f(x)
 *  oprType  - f
 *  vars[0]  - x
 */
struct unaryOperation
{
    unaryOprType oprType;
    VarName vars[1];
};

/**
 *  Binary operation.
 * 
 *  Operation: f(x, y)
 *  oprType  - f
 *  vars[0]  - x
 *  vars[1]  - y
 */
struct binaryOperation
{
    binaryOprType oprType;
    VarName vars[2];
};

/**
 *  Ternary operation.
 * 
 *  Operation: f(x, y, z)
 *  oprType  - f
 *  vars[0]  - x
 *  vars[1]  - y
 *  vars[2]  - z
 */
struct ternaryOperation
{
    ternaryOprType oprType;
    VarName vars[3];
};

/**
 *  Read an element from a multi-dimensional array.
 * 
 *  Operation: e = ar[x][y][ ... ][z]
 *  tgtName  - e
 *  arName   - ar
 *  idxs     - x, y, ..., z
 */
struct readArrayOperation
{
    VarName tgtName;
    VarName arName;
    std::vector<VarName> idxs;
};

/**
 *  Write an element to a multi-dimensional array.
 * 
 *  Operation: ar[x][y][ ... ][z] = s
 *  arName   - ar
 *  idxs     - x, y, ..., z
 *  srcName  - s
 */
struct writeArrayOperation
{
    VarName arName;
    std::vector<VarName> idxs;
    VarName srcName;
};

/**
 *  Call a function.
 * 
 *  Operation: call f( ... )
 *  func   - f
 *  varVec - ...
 */
struct funcCallOperation
{
    FuncName func;
    std::vector<VarName> varVec;
};

/**
 *  Return from a function with no return value.
 * 
 *  Operation: return x
 *  vars[0] - x
 */
struct funcRetVoidOperation
{
    /* Intentionally left as blank. */
};

/**
 *  Return from a function with return value.
 * 
 *  Operation: return x
 *  vars[0] - x
 */
struct funcRetValOperation
{
    VarName vars[1];
};

/**
 *  `Continue` a loop.
 */
struct loopContOperation
{
    /* Intentionally left as blank. */
};

/**
 *  Break a loop.
 */
struct loopBrkOperation
{
    /* Intentionally left as blank. */
};

/**
 *  Check if the condition of loop is met. If not, exit the loop.
 */
struct loopGuardOperation
{
    VarName testVar;
};

/**
 *  A sequence of commands inside a loop.
 */
struct loopBlkOperation
{
    cmdSeq cmdseq;
};

/**
 *  Enter a sequence of if-elif-elif-...-else test.
 */
struct branchBlkOperation
{
    cmdSeq cmdseq;
};

/**
 *  Exit from a sequence of if-elif-elif-...-else statement,
 *  when one of the sub-block has been finished execution.
 */
struct branchBrkOperation
{
    /* Intentionally left as blank. */
};

/**
 *  Enter a block of commands. This is designed to represent the
 *  statements between a pair of `{ ... }`. It is useful for life
 *  time management of variables.
 */
struct normalBlkOperation
{
    cmdSeq cmdseq;
};

/**
 *  Enter a block of commands if the test variable is not zero.
 */
struct condBlkOperation
{
    VarName testVar;
    cmdSeq cmdseq;
};

/**
 *  Declare a new variable.
 */
struct declVarOperation
{
    TypeName type;
    VarName var;
};

struct command
{
    cmdType type;
    void *opr;

    inline explicit command(cmdType type,
                            std::unique_ptr<unaryOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<binaryOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<ternaryOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<readArrayOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<writeArrayOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<funcCallOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<funcRetVoidOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<funcRetValOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<loopContOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<loopBrkOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<loopGuardOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<loopBlkOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<branchBlkOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<branchBrkOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<normalBlkOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<condBlkOperation> opr);
    inline explicit command(cmdType type,
                            std::unique_ptr<declVarOperation> opr);
    inline command() : type(cmdType::empty), opr(nullptr) {}
    ~command();
    command(command &&other);
    command(const command &other) = delete;
    command& operator=(command &&other);
    command& operator=(const command &other) = delete;
    void releaseMemory();
};

/*******************************************************************
 *              Implementation of Inline Functions                 *
 ******************************************************************/

command::command(cmdType _type, std::unique_ptr<unaryOperation> _opr)
{
    iferr (_type != cmdType::unaryOperation)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating unary operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating unary operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<binaryOperation> _opr)
{
    iferr (_type != cmdType::binaryOperation)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating binary operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating binary operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<ternaryOperation> _opr)
{
    iferr (_type != cmdType::ternaryOperation)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating ternary operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating ternary operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<readArrayOperation> _opr)
{
    iferr (_type != cmdType::readArrayOperation)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating read array operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating read array operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<writeArrayOperation> _opr)
{
    iferr (_type != cmdType::writeArrayOperation)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating write array operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating write array operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<funcCallOperation> _opr)
{
    iferr (_type != cmdType::functionCall)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating function call operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating function call operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<funcRetVoidOperation> _opr)
{
    iferr (_type != cmdType::functionReturnVoid)
        throw cmdCreationError("Error: argument mismatch while"
                               " creating function return void operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument while"
                               " creating function return void operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<funcRetValOperation> _opr)
{
    iferr (_type != cmdType::functionReturnVal)
        throw cmdCreationError("Error: argument mismatch while"
                               " creating function return value operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument while"
                               " creating function return value operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<loopContOperation> _opr)
{
    iferr (_type != cmdType::loopContinue)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating loop continue operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating loop continue operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<loopBrkOperation> _opr)
{
    iferr (_type != cmdType::loopBreak)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating loop break operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating loop break operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<loopGuardOperation> _opr)
{
    iferr (_type != cmdType::loopGuard)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating loop guard operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating loop guard operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<loopBlkOperation> _opr)
{
    iferr (_type != cmdType::loopBlock)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating loop block operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating loop block operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<branchBlkOperation> _opr)
{
    iferr (_type != cmdType::branchBlock)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating branch block operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating branch block operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<branchBrkOperation> _opr)
{
    iferr (_type != cmdType::branchBreak)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating branch break operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating branch break operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<normalBlkOperation> _opr)
{
    iferr (_type != cmdType::normalBlock)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating normal block operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating normal block operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<condBlkOperation> _opr)
{
    iferr (_type != cmdType::conditionalBlock)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating conditional block operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating conditional block operation!");

    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<declVarOperation> _opr)
{
    iferr (_type != cmdType::declareVariable)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating declare variable operation!");
    iferr (!static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating declare variable operation!");

    type = _type;
    opr = _opr.release();
}

}  // namespace cint

#endif  // SRC_INC_INTERMEDIATE_COMMAND_HH_
