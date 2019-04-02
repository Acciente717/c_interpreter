#ifndef __INTERMEDIATE_COMMAND_HH__
#define __INTERMEDIATE_COMMAND_HH__

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

using varName = std::string;
using funcName = std::string;

struct cmdSeq;

/*******************************************************************
 *                   Definition of Structures                      *
 ******************************************************************/

/**
 *  Types of all possible intermediate commands.
 */
enum class cmdType
{
    unaryOperation,
    binaryOperation,
    ternaryOperation,
    functionCall,
    functionReturn,
    loopContinue,
    loopBreak,
    branchBreak,
    branchBlock,
    normalBlock,
    conditionalBlock
};

/**
 *  Types of all possible unary operations.
 */
enum class unaryOprType
{

};

/**
 *  Types of all possible binary operations.
 */
enum class binaryOprType
{

};

/**
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
    assignNotEqual
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
    varName vars[1];
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
    varName vars[2];
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
    varName vars[3];
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
    funcName func;
    std::vector<varName> varVec;
};

/**
 *  Return from a function.
 * 
 *  Operation: return x
 *  vars[0] - x
 */
struct funcRetOperation
{
    varName vars[1];
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
    varName testVar;
    cmdSeq cmdseq;
};

struct command
{
    cmdType type;
    void *opr;
    
    inline explicit command(cmdType type, std::unique_ptr<unaryOperation> opr);
    inline explicit command(cmdType type, std::unique_ptr<binaryOperation> opr);
    inline explicit command(cmdType type, std::unique_ptr<ternaryOperation> opr);
    inline explicit command(cmdType type, std::unique_ptr<funcCallOperation> opr);
    inline explicit command(cmdType type, std::unique_ptr<funcRetOperation> opr);
    inline explicit command(cmdType type, std::unique_ptr<loopContOperation> opr);
    inline explicit command(cmdType type, std::unique_ptr<loopBrkOperation> opr);
    inline explicit command(cmdType type, std::unique_ptr<branchBlkOperation> opr);
    inline explicit command(cmdType type, std::unique_ptr<branchBrkOperation> opr);
    inline explicit command(cmdType type, std::unique_ptr<normalBlkOperation> opr);
    inline explicit command(cmdType type, std::unique_ptr<condBlkOperation> opr);
    ~command();
    command(command &&other) = delete;
    command(const command &other) = delete;
    command& operator=(command &&other) = delete;
    command& operator=(const command &other) = delete;
};

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

/*******************************************************************
 *              Implementation of Inline Functions                 *
 ******************************************************************/

command::command(cmdType _type, std::unique_ptr<unaryOperation> _opr)
{
    iferr (type != cmdType::unaryOperation)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating unary operation!");
    iferr (static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating unary operation!");
    
    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<binaryOperation> _opr)
{
    iferr (type != cmdType::binaryOperation)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating binary operation!");
    iferr (static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating binary operation!");
    
    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<ternaryOperation> _opr)
{
    iferr (type != cmdType::ternaryOperation)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating ternary operation!");
    iferr (static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating ternary operation!");
    
    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<funcCallOperation> _opr)
{
    iferr (type != cmdType::functionCall)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating function call operation!");
    iferr (static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating function call operation!");
    
    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<funcRetOperation> _opr)
{
    iferr (type != cmdType::functionReturn)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating function return operation!");
    iferr (static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating function return operation!");
    
    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<loopContOperation> _opr)
{
    iferr (type != cmdType::loopContinue)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating loop continue operation!");
    iferr (static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating loop continue operation!");
    
    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<loopBrkOperation> _opr)
{
    iferr (type != cmdType::loopBreak)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating loop break operation!");
    iferr (static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating loop break operation!");
    
    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<branchBlkOperation> _opr)
{
    iferr (type != cmdType::branchBlock)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating branch block operation!");
    iferr (static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating branch block operation!");
    
    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<branchBrkOperation> _opr)
{
    iferr (type != cmdType::branchBreak)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating branch break operation!");
    iferr (static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating branch break operation!");
    
    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<normalBlkOperation> _opr)
{
    iferr (type != cmdType::loopBreak)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating normal block operation!");
    iferr (static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating normal block operation!");
    
    type = _type;
    opr = _opr.release();
}

command::command(cmdType _type, std::unique_ptr<condBlkOperation> _opr)
{
    iferr (type != cmdType::loopBreak)
        throw cmdCreationError("Error: argument mismatch"
                               " while creating conditional block operation!");
    iferr (static_cast<bool>(_opr))
        throw cmdCreationError("Error: invalid pointer as argument"
                               " while creating conditional block operation!");
    
    type = _type;
    opr = _opr.release();
}

}

#endif
