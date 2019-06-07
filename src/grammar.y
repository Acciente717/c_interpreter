%{

#include <string>
#include <unordered_set>
#include <memory>
#include <stack>
#include <cassert>

#include "intermediateCommand.hh"
#include "yaccInfoStructure.hh"
#include "utils.hh"
#include "typeManager.hh"
#include "functionManager.hh"
#include "executionManager.hh"

#define YYSTYPE cint::yaccInfo
#define TEMP_NAME_LEN 8

#define TERNARY_ARITHMIC_OPERATION(type, $$, $1, $3)\
using namespace cint;\
std::string tmpVar;\
while (isTempNameExist(tmpVar = genRandomStr(TEMP_NAME_LEN, true), *gpExistTmpVar));\
cmdSeqStk.top()->cmds.emplace_back(cmdType::ternaryOperation,\
                            std::unique_ptr<ternaryOperation>\
                            (new ternaryOperation\
                            {\
                                ternaryOprType::type,\
                                tmpVar,\
                                *reinterpret_cast<std::string *>($1.data),\
                                *reinterpret_cast<std::string *>($3.data)\
                            }));\
$$ = cint::yaccInfo(yaccInfo::infoType::varName,\
                    std::move(tmpVar))

cint::basicTypesEnum gActiveTypeNum;

std::vector<long> gFuncParamTypes;
std::vector<std::string> gFuncParamNames;
std::vector<std::string> gFuncArgNames;

std::stack<cint::cmdSeq *> cmdSeqStk;
std::unordered_set<std::string> *gpExistTmpVar;

std::string gLexIdentifier;
std::string gLexInteger;
std::string gLexFloat;
std::vector<std::string> gSubscripts;

extern "C"
{
        int yyparse(void);
        int yylex(void);
        int yyerror(const char *errmsg)
        {
            std::cout << errmsg << std::endl;
            return 0;
        }
        int yywrap();
}
%}

 /* Reserved Type Keywords */
%token CINT_TYPE_LONG CINT_TYPE_FLOAT CINT_TYPE_VOID

 /* Reserved Control Keywords */
%token CINT_CTRL_IF CINT_CTRL_ELSE CINT_CTRL_WHILE CINT_CTRL_BREAK 
%token CINT_CTRL_CONTINUE CINT_CTRL_RETURN

 /* Operators */
%token CINT_OPR_ADD CINT_OPR_SUB CINT_OPR_MUL CINT_OPR_DIV
%token CINT_OPR_MOD CINT_OPR_LOGIC_AND CINT_OPR_LOGIC_OR
%token CINT_OPR_LT CINT_OPR_EQ CINT_OPR_GT CINT_OPR_NE
%token CINT_OPR_LE CINT_OPR_GE
%token CINT_OPR_ASSIGN CINT_OPR_NOT

 /* Delimiters */
%token CINT_DELIM_LPAREN CINT_DELIM_RPAREN
%token CINT_DELIM_LBRACE CINT_DELIM_RBRACE
%token CINT_DELIM_LBRACKET CINT_DELIM_RBRACKET
%token CINT_DELIM_SEMICOLON CINT_DELIM_COMMA

 /* Identifier */
%token CINT_IDENTIFIER

 /* Integer */
%token CINT_INTEGER

 /* Float */
%token CINT_FLOAT

%start start

%%

start                       : /* empty */
                            | start ND_FUNCTION_DEFINITION
                            ;


 /* A Block of statements. { ... } */
ND_BLOCK_STATEMENTS         : CINT_DELIM_LBRACE
                                {
                                    cmdSeqStk.push(new cint::cmdSeq);
                                    cmdSeqStk.top()->type = cint::cmdSeqType::normal;
                                    gpExistTmpVar = new std::unordered_set<std::string>;
                                }
                              ND_STATEMENT_SEQUENCE CINT_DELIM_RBRACE
                            ;


 /* Function definition. */
ND_FUNCTION_DEFINITION      : ND_TYPE_NAME ND_IDENTIFIER ND_PARAM_LIST ND_BLOCK_STATEMENTS
                                {
                                    cmdSeqStk.top()->type = cint::cmdSeqType::function;
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::functionReturnVoid,
                                                                       std::unique_ptr<cint::funcRetVoidOperation>
                                                                       (new cint::funcRetVoidOperation()));
                                    cint::getFuncMgr().defineFunction(
                                        *reinterpret_cast<std::string *>($2.data),
                                        std::move(gFuncParamTypes),
                                        std::move(gFuncParamNames),
                                        cint::getTypeMgr().getTypeNumByName(
                                            *reinterpret_cast<std::string*>($1.data)
                                        ),
                                        std::move(*cmdSeqStk.top())
                                    );
                                    
                                    gFuncParamNames.clear();
                                    gFuncParamTypes.clear();
                                    cmdSeqStk.pop();
                                    assert(cmdSeqStk.size() == 0);
                                }
                            ;


 /* A sequence of statements. */
ND_STATEMENT_SEQUENCE       : /* empty */
                            | ND_STATEMENT_SEQUENCE ND_STATEMENT
                            ;

 /* Parameter list. */
ND_PARAM_LIST               : CINT_DELIM_LPAREN CINT_DELIM_RPAREN
                            | CINT_DELIM_LPAREN ND_NON_EMPTY_PARAM_LIST CINT_DELIM_RPAREN
                            ;

 /* Non empty parameter list. */
ND_NON_EMPTY_PARAM_LIST     : ND_TYPE_NAME ND_IDENTIFIER
                                {
                                    gFuncParamTypes.emplace_back(
                                        cint::getTypeMgr().getTypeNumByName(
                                        *reinterpret_cast<std::string *>($1.data))
                                    );
                                    gFuncParamNames.emplace_back(
                                        *reinterpret_cast<std::string *>($2.data)
                                    );
                                }
                            | ND_NON_EMPTY_PARAM_LIST CINT_DELIM_COMMA ND_TYPE_NAME ND_IDENTIFIER
                                {
                                    gFuncParamTypes.emplace_back(
                                        cint::getTypeMgr().getTypeNumByName(
                                        *reinterpret_cast<std::string *>($3.data))
                                    );
                                    gFuncParamNames.emplace_back(
                                        *reinterpret_cast<std::string *>($4.data)
                                    );
                                }
                            ;


 /* A statement. */
ND_STATEMENT                : ND_DECLARE_VARIABLE
                            | ND_INITIALIZE_VARIABLE
                            | ND_DECLARE_ARRAY
                            | ND_ARITHMIC_OPERATION
                            | ND_CONTROL_COMMAND
                            | ND_BLOCK_STATEMENTS
                                {
                                    auto subCmds = cmdSeqStk.top();
                                    cmdSeqStk.pop();
                                    std::unique_ptr<cint::normalBlkOperation> ptr(
                                        new cint::normalBlkOperation{std::move(*subCmds)});
                                    delete subCmds;
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::normalBlock,
                                                                       std::move(ptr));
                                }
                            | ND_BRANCH_STATEMENTS
                                {
                                    auto subCmds = cmdSeqStk.top();
                                    cmdSeqStk.pop();
                                    std::unique_ptr<cint::branchBlkOperation> ptr(
                                        new cint::branchBlkOperation{std::move(*subCmds)});
                                    delete subCmds;
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::branchBlock,
                                                                       std::move(ptr));
                                }
                            | ND_LOOP_STATEMENTS
                                {
                                    auto subCmds = cmdSeqStk.top();
                                    cmdSeqStk.pop();
                                    std::unique_ptr<cint::loopBlkOperation> ptr(
                                        new cint::loopBlkOperation{std::move(*subCmds)});
                                    delete subCmds;
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::loopBlock,
                                                                       std::move(ptr));
                                }
                            ;


 /* Invoke function. */
ND_FUNCTION_INVOCATION      : ND_IDENTIFIER CINT_DELIM_LPAREN ND_ARGUMENT_LIST CINT_DELIM_RPAREN
                                {
                                    cmdSeqStk.top()->cmds.emplace_back(
                                        cint::cmdType::functionCall,
                                        std::unique_ptr<cint::funcCallOperation>(
                                            new cint::funcCallOperation{
                                                *reinterpret_cast<std::string *>($1.data),
                                                std::move(gFuncArgNames)
                                            }
                                        )
                                    );
                                    gFuncArgNames.clear();
                                    std::string tmpVar;
                                    while (cint::isTempNameExist(tmpVar = 
                                                                    cint::genRandomStr(TEMP_NAME_LEN, true),
                                                                                       *gpExistTmpVar));
                                    cmdSeqStk.top()->cmds.emplace_back(
                                        cint::cmdType::unaryOperation,
                                        std::unique_ptr<cint::unaryOperation>(
                                            new cint::unaryOperation{
                                                cint::unaryOprType::getReturnValue,
                                                {tmpVar}
                                            }
                                        )
                                    );
                                    $$ = cint::yaccInfo(cint::yaccInfo::infoType::varName,
                                                        std::move(tmpVar));
                                }
                            ;

 /* Argument list. */
ND_ARGUMENT_LIST            : /* empty */
                            | ND_NON_EMPTY_ARG_LIST
                            ;

 /* Non empty argument list. */
ND_NON_EMPTY_ARG_LIST       : ND_EXPRESSION
                                {
                                    gFuncArgNames.emplace_back(
                                        *reinterpret_cast<std::string *>($1.data)
                                    );
                                }
                            | ND_NON_EMPTY_ARG_LIST CINT_DELIM_COMMA ND_EXPRESSION
                                {
                                    gFuncArgNames.emplace_back(
                                        *reinterpret_cast<std::string *>($3.data)
                                    );
                                }
                            ;


 /* Loop statements. */
ND_LOOP_STATEMENTS          : CINT_CTRL_WHILE
                                {
                                    cmdSeqStk.push(new cint::cmdSeq);
                                    cmdSeqStk.top()->type = cint::cmdSeqType::loop;
                                }
                              CINT_DELIM_LPAREN ND_EXPRESSION CINT_DELIM_RPAREN
                                {
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::loopGuard,
                                                                       std::unique_ptr<cint::loopGuardOperation>
                                                                       (new cint::loopGuardOperation
                                                                       {*reinterpret_cast<std::string *>($4.data)}));
                                }
                              CINT_DELIM_LBRACE ND_STATEMENT_SEQUENCE CINT_DELIM_RBRACE
                                {
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::loopContinue,
                                                               std::unique_ptr<cint::loopContOperation>
                                                               (new cint::loopContOperation()));
                                }
                            ;


 /* Branch statement. */
ND_BRANCH_STATEMENTS        :   
                                {
                                    cmdSeqStk.push(new cint::cmdSeq);
                                    cmdSeqStk.top()->type = cint::cmdSeqType::branch;
                                }
                              ND_BRANCH_IF_STATEMENT ND_BRANCH_ELIF_SEQUENCE ND_BRANCH_ELSE_STATEMENT
                            ;

 /* If statement. */
ND_BRANCH_IF_STATEMENT      : CINT_CTRL_IF
                              CINT_DELIM_LPAREN ND_EXPRESSION CINT_DELIM_RPAREN ND_BLOCK_STATEMENTS
                                {
                                    auto subCmds = cmdSeqStk.top();
                                    cmdSeqStk.pop();
                                    subCmds->cmds.emplace_back(cint::cmdType::branchBreak,
                                                               std::unique_ptr<cint::branchBrkOperation>
                                                               (new cint::branchBrkOperation()));
                                    std::unique_ptr<cint::condBlkOperation> ptr(
                                        new cint::condBlkOperation{*reinterpret_cast<std::string *>($3.data),
                                                                   std::move(*subCmds)});
                                    delete subCmds;
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::conditionalBlock,
                                                                       std::move(ptr));
                                }
                            ;

 /* A sequence of else-if statements. */
ND_BRANCH_ELIF_SEQUENCE     : /* empty */
                            | ND_BRANCH_ELIF_SEQUENCE CINT_CTRL_ELSE CINT_CTRL_IF
                              CINT_DELIM_LPAREN ND_EXPRESSION CINT_DELIM_RPAREN ND_BLOCK_STATEMENTS
                                {
                                    auto subCmds = cmdSeqStk.top();
                                    cmdSeqStk.pop();
                                    subCmds->cmds.emplace_back(cint::cmdType::branchBreak,
                                                               std::unique_ptr<cint::branchBrkOperation>
                                                               (new cint::branchBrkOperation()));
                                    std::unique_ptr<cint::condBlkOperation> ptr(
                                        new cint::condBlkOperation{*reinterpret_cast<std::string *>($5.data),
                                                                   std::move(*subCmds)});
                                    delete subCmds;
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::conditionalBlock,
                                                                       std::move(ptr));
                                }
                            ;

 /* Else statement. */
ND_BRANCH_ELSE_STATEMENT    : /* empty */
                            | CINT_CTRL_ELSE ND_BLOCK_STATEMENTS
                                {
                                    auto subCmds = cmdSeqStk.top();
                                    cmdSeqStk.pop();
                                    subCmds->cmds.emplace_back(cint::cmdType::branchBreak,
                                                               std::unique_ptr<cint::branchBrkOperation>
                                                               (new cint::branchBrkOperation()));
                                    std::unique_ptr<cint::normalBlkOperation> ptr(
                                        new cint::normalBlkOperation{std::move(*subCmds)});
                                    delete subCmds;
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::normalBlock,
                                                                       std::move(ptr));
                                }
                            ;


 /* Declare a variable. */
ND_DECLARE_VARIABLE         : ND_TYPE_NAME ND_IDENTIFIER CINT_DELIM_SEMICOLON
                                {
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::declareVariable,
                                                                        std::unique_ptr<cint::declVarOperation>
                                                                        (new cint::declVarOperation{
                                                                        *reinterpret_cast<std::string *>($1.data),
                                                                        *reinterpret_cast<std::string *>
                                                                                                ($2.data)}));
                                }
                            ;


 /* Declare an array. */
ND_DECLARE_ARRAY            : ND_TYPE_NAME ND_IDENTIFIER ND_INDEX_LIST CINT_DELIM_SEMICOLON
                                {
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::declareArray,
                                                                        std::unique_ptr<cint::declArrOperation>
                                                                        (new cint::declArrOperation{
                                                                        *reinterpret_cast<std::string *>($1.data),
                                                                        *reinterpret_cast<std::string *>
                                                                                                ($2.data),
                                                                        gSubscripts}));
                                }
                            ;

 /* Index list. */
ND_INDEX_LIST               : CINT_DELIM_LBRACKET ND_EXPRESSION CINT_DELIM_RBRACKET
                                {
                                    gSubscripts.clear();
                                    gSubscripts.emplace_back(std::move(*reinterpret_cast<std::string *>($2.data)));
                                }
                            | ND_INDEX_LIST CINT_DELIM_LBRACKET ND_EXPRESSION CINT_DELIM_RBRACKET
                                {
                                    gSubscripts.emplace_back(std::move(*reinterpret_cast<std::string *>($3.data)));
                                }
                            ;


 /* Initialize a variable. */
ND_INITIALIZE_VARIABLE      : ND_TYPE_NAME ND_IDENTIFIER CINT_OPR_ASSIGN ND_EXPRESSION CINT_DELIM_SEMICOLON
                                {
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::declareVariable,
                                                                        std::unique_ptr<cint::declVarOperation>
                                                                        (new cint::declVarOperation{
                                                                        *reinterpret_cast<std::string *>($1.data),
                                                                        *reinterpret_cast<std::string *>
                                                                                                ($2.data)}));
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::binaryOperation,
                                                                        std::unique_ptr<cint::binaryOperation>
                                                                        (new cint::binaryOperation{
                                                                        cint::binaryOprType::assignVariable,
                                                                        {*reinterpret_cast<std::string *>($2.data),
                                                                        *reinterpret_cast<std::string *>
                                                                                                ($4.data)}}));
                                }
                            ;


 /* All type names. */
ND_TYPE_NAME                : CINT_TYPE_LONG
                                {
                                    using namespace cint;
                                    $$ = yaccInfo(yaccInfo::infoType::typeName,
                                                  "long");
                                }
                            | CINT_TYPE_FLOAT
                                {
                                    using namespace cint;
                                    $$ = yaccInfo(yaccInfo::infoType::typeName,
                                                  "float");
                                }
                            | CINT_TYPE_VOID
                                {
                                    using namespace cint;
                                    $$ = yaccInfo(yaccInfo::infoType::typeName,
                                                  "void");
                                }
                            ;


 /* All control commands. */
ND_CONTROL_COMMAND          : CINT_CTRL_BREAK CINT_DELIM_SEMICOLON
                                {
                                    using namespace cint;
                                    cmdSeqStk.top()->cmds.emplace_back(cmdType::loopBreak,
                                                                       std::unique_ptr<loopBrkOperation>
                                                                       (new loopBrkOperation()));
                                }
                            | CINT_CTRL_CONTINUE CINT_DELIM_SEMICOLON
                                {
                                    using namespace cint;
                                    cmdSeqStk.top()->cmds.emplace_back(cmdType::loopContinue,
                                                                       std::unique_ptr<loopContOperation>
                                                                       (new loopContOperation()));
                                }
                            | CINT_CTRL_RETURN CINT_DELIM_SEMICOLON
                                {
                                    using namespace cint;
                                    cmdSeqStk.top()->cmds.emplace_back(cmdType::functionReturnVoid,
                                                                       std::unique_ptr<funcRetVoidOperation>
                                                                       (new funcRetVoidOperation()));
                                }
                            | CINT_CTRL_RETURN ND_EXPRESSION CINT_DELIM_SEMICOLON
                                {
                                    using namespace cint;
                                    cmdSeqStk.top()->cmds.emplace_back(cmdType::functionReturnVal,
                                                                       std::unique_ptr<funcRetValOperation>
                                                                       (new funcRetValOperation{
                                                                           *reinterpret_cast<std::string*>
                                                                           ($2.data)
                                                                       }));
                                }
                            ;

 /* Do arithmic operation. */
ND_ARITHMIC_OPERATION       : ND_IDENTIFIER CINT_OPR_ASSIGN ND_EXPRESSION CINT_DELIM_SEMICOLON
                                {
                                    using namespace cint;
                                    cmdSeqStk.top()->cmds.emplace_back(cmdType::binaryOperation,
                                                                std::unique_ptr<binaryOperation>
                                                                (new binaryOperation
                                                                {
                                                                    binaryOprType::assignVariable,
                                                                    *reinterpret_cast<std::string *>($1.data),
                                                                    *reinterpret_cast<std::string *>($3.data)
                                                                }));
                                }
                            | ND_ARRAY_ELEMENT CINT_OPR_ASSIGN ND_EXPRESSION CINT_DELIM_SEMICOLON
                                {
                                    using namespace cint;
                                    cmdSeqStk.top()->cmds.emplace_back(cmdType::writeArrayOperation,
                                    std::unique_ptr<writeArrayOperation>(
                                        new writeArrayOperation{
                                            std::move(*reinterpret_cast<std::string *>
                                                        ($1.data)),
                                            gSubscripts,
                                            std::move(*reinterpret_cast<std::string *>
                                                        ($3.data)),
                                        }
                                    ));
                                }
                            | ND_EXPRESSION CINT_DELIM_SEMICOLON
                            ;


 /* Expression hierachy. Lowest 13, Highest 1 */
ND_EXPRESSION               : ND_EXPRESSION_14
                                {
                                    $$ = $1;
                                }
                            ;


ND_EXPRESSION_14            : ND_EXPRESSION_13
                                {
                                    $$ = $1;
                                }
                            ;

ND_EXPRESSION_13            : ND_EXPRESSION_12
                                {
                                    $$ = $1;
                                }
                            ;

ND_EXPRESSION_12            : ND_EXPRESSION_11
                                {
                                    $$ = $1;
                                }
                            | ND_EXPRESSION_12 CINT_OPR_LOGIC_OR ND_EXPRESSION_11
                                {
                                    TERNARY_ARITHMIC_OPERATION(assignLogicOr, $$, $1, $3);
                                }
                            ;

ND_EXPRESSION_11            : ND_EXPRESSION_10
                                {
                                    $$ = $1;
                                }
                            | ND_EXPRESSION_11 CINT_OPR_LOGIC_AND ND_EXPRESSION_10
                                {
                                    TERNARY_ARITHMIC_OPERATION(assignLogicAnd, $$, $1, $3);
                                }
                            ;

ND_EXPRESSION_10            : ND_EXPRESSION_9
                                {
                                    $$ = $1;
                                }
                            ;

ND_EXPRESSION_9             : ND_EXPRESSION_8
                                {
                                    $$ = $1;
                                }
                            ;

ND_EXPRESSION_8             : ND_EXPRESSION_7
                                {
                                    $$ = $1;
                                }
                            ;

ND_EXPRESSION_7             : ND_EXPRESSION_6
                                {
                                    $$ = $1;
                                }
                            | ND_EXPRESSION_7 CINT_OPR_EQ ND_EXPRESSION_6
                                {
                                    TERNARY_ARITHMIC_OPERATION(assignEqual, $$, $1, $3);
                                }
                            | ND_EXPRESSION_7 CINT_OPR_NE ND_EXPRESSION_6
                                {
                                    TERNARY_ARITHMIC_OPERATION(assignNotEqual, $$, $1, $3);
                                }
                            ;

ND_EXPRESSION_6             : ND_EXPRESSION_5
                                {
                                    $$ = $1;
                                }
                            | ND_EXPRESSION_6 CINT_OPR_LT ND_EXPRESSION_5
                                {
                                    TERNARY_ARITHMIC_OPERATION(assignLess, $$, $1, $3);
                                }
                            | ND_EXPRESSION_6 CINT_OPR_LE ND_EXPRESSION_5
                                {
                                    TERNARY_ARITHMIC_OPERATION(assignLessEqual, $$, $1, $3);
                                }
                            | ND_EXPRESSION_6 CINT_OPR_GT ND_EXPRESSION_5
                                {
                                    TERNARY_ARITHMIC_OPERATION(assignGreater, $$, $1, $3);
                                }
                            | ND_EXPRESSION_6 CINT_OPR_GE ND_EXPRESSION_5
                                {
                                    TERNARY_ARITHMIC_OPERATION(assignGreaterEqual, $$, $1, $3);
                                }
                            ;

ND_EXPRESSION_5             : ND_EXPRESSION_4
                                {
                                    $$ = $1;
                                }
                            ;

ND_EXPRESSION_4             : ND_EXPRESSION_3
                                {
                                    $$ = $1;
                                }
                            | ND_EXPRESSION_4 CINT_OPR_ADD ND_EXPRESSION_3
                                {
                                    TERNARY_ARITHMIC_OPERATION(assignSum, $$, $1, $3);
                                }
                            | ND_EXPRESSION_4 CINT_OPR_SUB ND_EXPRESSION_3
                                {
                                    TERNARY_ARITHMIC_OPERATION(assignDifference, $$, $1, $3);
                                }
                            ;

ND_EXPRESSION_3             : ND_EXPRESSION_2
                                {
                                    $$ = $1;
                                }
                            | ND_EXPRESSION_3 CINT_OPR_MUL ND_EXPRESSION_2
                                {
                                    TERNARY_ARITHMIC_OPERATION(assignProduct, $$, $1, $3);
                                }
                            | ND_EXPRESSION_3 CINT_OPR_DIV ND_EXPRESSION_2
                                {
                                    TERNARY_ARITHMIC_OPERATION(assignQuotient, $$, $1, $3);
                                }
                            | ND_EXPRESSION_3 CINT_OPR_MOD ND_EXPRESSION_2
                                {
                                    TERNARY_ARITHMIC_OPERATION(assignResidue, $$, $1, $3);
                                }
                            ;

ND_EXPRESSION_2             : ND_EXPRESSION_1
                                {
                                    $$ = $1;
                                }
                            | CINT_OPR_SUB ND_EXPRESSION_2
                                {
                                    using namespace cint;
                                    std::string tmpVar;
                                    while (isTempNameExist(tmpVar = genRandomStr(TEMP_NAME_LEN, true), *gpExistTmpVar));
                                    cmdSeqStk.top()->cmds.emplace_back(cmdType::binaryOperation,
                                                                std::unique_ptr<binaryOperation>
                                                                (new binaryOperation
                                                                {
                                                                    binaryOprType::assignNegate,
                                                                    {
                                                                        tmpVar,
                                                                        *reinterpret_cast<std::string *>($2.data)
                                                                    }
                                                                }));
                                    $$ = cint::yaccInfo(yaccInfo::infoType::varName,
                                                        std::move(tmpVar));
                                }
                            | CINT_OPR_NOT ND_EXPRESSION_2
                                {
                                    using namespace cint;
                                    std::string tmpVar;
                                    while (isTempNameExist(tmpVar = genRandomStr(TEMP_NAME_LEN, true), *gpExistTmpVar));
                                    cmdSeqStk.top()->cmds.emplace_back(cmdType::binaryOperation,
                                                                std::unique_ptr<binaryOperation>
                                                                (new binaryOperation
                                                                {
                                                                    binaryOprType::assignNot,
                                                                    {
                                                                        tmpVar,
                                                                        *reinterpret_cast<std::string *>($2.data)
                                                                    }
                                                                }));
                                    $$ = cint::yaccInfo(yaccInfo::infoType::varName,
                                                        std::move(tmpVar));
                                }
                            ;

ND_EXPRESSION_1             : ND_IDENTIFIER
                                {
                                    $$ = $1;
                                }
                            | CINT_DELIM_LPAREN ND_EXPRESSION CINT_DELIM_RPAREN
                                {
                                    $$ = $2;
                                }
                            | ND_ARRAY_ELEMENT
                                {
                                    using namespace cint;
                                    std::string tmpVar;
                                    while (isTempNameExist(tmpVar = genRandomStr(TEMP_NAME_LEN, true), *gpExistTmpVar));
                                    cmdSeqStk.top()->cmds.emplace_back(cmdType::readArrayOperation,
                                                                       std::unique_ptr<readArrayOperation>(
                                                                           new readArrayOperation{
                                                                                tmpVar,
                                                                                std::move(*reinterpret_cast<std::string *>
                                                                                         ($1.data)),
                                                                                gSubscripts
                                                                           }
                                                                       ));
                                    $$ = yaccInfo(yaccInfo::infoType::varName,
                                                  std::move(tmpVar));
                                }
                            | ND_STANDALONE_LITERAL
                                {
                                    $$ = $1;
                                }
                            | ND_FUNCTION_INVOCATION
                                {
                                    $$ = $1;
                                }
                            ;

 /* Literals used as a constant variable. */
ND_STANDALONE_LITERAL       : CINT_INTEGER
                                {
                                    std::string tmpVar;
                                    while (cint::isTempNameExist(tmpVar = cint::genRandomStr(TEMP_NAME_LEN, true),
                                                                *gpExistTmpVar));
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::declareVariable,
                                                                        std::unique_ptr<cint::declVarOperation>
                                                                        (new cint::declVarOperation{
                                                                        "long", tmpVar}));
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::binaryOperation,
                                                                        std::unique_ptr<cint::binaryOperation>
                                                                        (new cint::binaryOperation{
                                                                        cint::binaryOprType::assignLiteral,
                                                                        {tmpVar, gLexInteger}}));
                                    $$ = cint::yaccInfo(cint::yaccInfo::infoType::varName,
                                                  std::move(tmpVar));
                                }
                            | CINT_FLOAT
                                {
                                    std::string tmpVar;
                                    while (cint::isTempNameExist(tmpVar = cint::genRandomStr(TEMP_NAME_LEN, true),
                                                                *gpExistTmpVar));
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::declareVariable,
                                                                        std::unique_ptr<cint::declVarOperation>
                                                                        (new cint::declVarOperation{
                                                                        "float", tmpVar}));
                                    cmdSeqStk.top()->cmds.emplace_back(cint::cmdType::binaryOperation,
                                                                        std::unique_ptr<cint::binaryOperation>
                                                                        (new cint::binaryOperation{
                                                                        cint::binaryOprType::assignLiteral,
                                                                        {tmpVar, gLexFloat}}));
                                    $$ = cint::yaccInfo(cint::yaccInfo::infoType::varName,
                                                  std::move(tmpVar));
                                }
                            ;


ND_ARRAY_ELEMENT            : ND_IDENTIFIER CINT_DELIM_LBRACKET ND_EXPRESSION CINT_DELIM_RBRACKET
                                {
                                    gSubscripts.clear();
                                    gSubscripts.emplace_back(std::move(*reinterpret_cast<std::string *>($3.data)));
                                    $$ = $1;
                                }
                            | ND_ARRAY_ELEMENT CINT_DELIM_LBRACKET ND_EXPRESSION CINT_DELIM_RBRACKET
                                {
                                    gSubscripts.emplace_back(std::move(*reinterpret_cast<std::string *>($3.data)));
                                    $$ = $1;
                                }
                            ;


ND_IDENTIFIER               : CINT_IDENTIFIER
                                {
                                    using namespace cint;
                                    $$ = yaccInfo(yaccInfo::infoType::varName,
                                                  gLexIdentifier);
                                }
                            ;

%%

#include "lex_parser.cc"

int yywrap()
{
    auto pAllFuncs = cint::getFuncMgr().getAllDefinedFuncs();
    for (const auto &i : *pAllFuncs)
    {
        std::cout << "function: " << i.first;
        std::cout << '(';
        for (long j = 0; j < i.second.paramNames.size(); ++j)
        {
            if (j != 0) std::cout << ", ";
            std::cout << cint::getTypeMgr().getTypenameByNum(i.second.paramTypeNums[j]);
            std::cout << ' ' << i.second.paramNames[j];
        }
        std::cout << ')' << std::endl;
        printCmdSeq(i.second.cmds, 1);
        std::cout << std::endl;
    }

    std::cout << std::endl
              << "### start program execution ###" << std::endl;

    cint::executionManager exeMgr("main");
    exeMgr.run();

    return 1;
}
