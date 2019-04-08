%{

#include <string>
#include <unordered_set>
#include <memory>

#include "intermediateCommand.hh"
#include "yaccInfoStructure.hh"
#include "utils.hh"

#define YYSTYPE cint::yaccInfo
#define TEMP_NAME_LEN 8

#define TERNARY_ARITHMIC_OPERATION(type, $$, $1, $3)\
using namespace cint;\
std::string tmpVar;\
while (isTempNameExist(tmpVar = genRandomStr(TEMP_NAME_LEN, true), *gpExistTmpVar));\
gpCmdSeq->cmds.emplace_back(cmdType::ternaryOperation,\
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

cint::cmdSeq *gpCmdSeq;
std::unordered_set<std::string> *gpExistTmpVar;

std::string gLexIdentifier;
std::string gLexInteger;

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
%token CINT_TYPE_INT CINT_TYPE_DOUBLE CINT_TYPE_VOID

 /* Reserved Control Keywords */
%token CINT_CTRL_IF CINT_CTRL_WHILE CINT_CTRL_BREAK 
%token CINT_CTRL_CONTINUE CINT_CTRL_RETURN

 /* Operators */
%token CINT_OPR_ADD CINT_OPR_SUB CINT_OPR_MUL CINT_OPR_DIV
%token CINT_OPR_MOD CINT_OPR_LOGIC_AND CINT_OPR_LOGIC_OR
%token CINT_OPR_LT CINT_OPR_EQ CINT_OPR_GT CINT_OPR_NE
%token CINT_OPR_LE CINT_OPR_GE
%token CINT_OPR_ASSIGN

 /* Delimiters */
%token CINT_DELIM_LPAREN CINT_DELIM_RPAREN
%token CINT_DELIM_LBRACE CINT_DELIM_RBRACE
%token CINT_DELIM_LBRACKET CINT_DELIM_RBRACKET
%token CINT_DELIM_SEMICOLON CINT_DELIM_COMMA

 /* Identifier */
%token CINT_IDENTIFIER

 /* Integer */
%token CINT_INTEGER

%start start

%%

start                       : /* empty */
                            | start ND_BLOCK_STATEMENTS
                            ;


 /* A Block of statements. { ... } */
ND_BLOCK_STATEMENTS         : CINT_DELIM_LBRACE
                                {
                                    gpCmdSeq = new cint::cmdSeq;
                                    gpCmdSeq->type = cint::cmdSeqType::normal;
                                    gpExistTmpVar = new std::unordered_set<std::string>;
                                }
                              ND_STATEMENT_SEQUENCE CINT_DELIM_RBRACE
                            ;


 /* A sequence of statements. */
ND_STATEMENT_SEQUENCE       : /* empty */
                            | ND_STATEMENT_SEQUENCE ND_STATEMENT
                            ;


 /* A statement. */
ND_STATEMENT                : ND_DECLARE_VARIABLE
                            | ND_ARITHMIC_OPERATION
                            | ND_CONTROL_COMMAND
                            ;

 /* Declare a variable. */
ND_DECLARE_VARIABLE         : ND_TYPE_NAME ND_IDENTIFIER CINT_DELIM_SEMICOLON
                            ;


 /* All type names. */
ND_TYPE_NAME                : CINT_TYPE_INT CINT_DELIM_SEMICOLON
                            | CINT_TYPE_DOUBLE CINT_DELIM_SEMICOLON
                            | CINT_TYPE_VOID CINT_DELIM_SEMICOLON
                            ;


 /* All control commands. */
ND_CONTROL_COMMAND          : CINT_CTRL_BREAK 
                            | CINT_CTRL_CONTINUE
                            | CINT_CTRL_RETURN
                            ;

 /* Do arithmic operation. */
ND_ARITHMIC_OPERATION       : ND_IDENTIFIER CINT_OPR_ASSIGN ND_EXPRESSION CINT_DELIM_SEMICOLON
                                {
                                    using namespace cint;
                                    gpCmdSeq->cmds.emplace_back(cmdType::binaryOperation,
                                                                std::unique_ptr<binaryOperation>
                                                                (new binaryOperation
                                                                {
                                                                    binaryOprType::assign,
                                                                    *reinterpret_cast<std::string *>($1.data),
                                                                    *reinterpret_cast<std::string *>($3.data)
                                                                }));
                                }
                            ;


 /* Expression hierachy. Lowest 13, Highest 1 */
ND_EXPRESSION               : ND_EXPRESSION_13
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
                            ;

ND_EXPRESSION_1             : ND_IDENTIFIER
                                {
                                    $$ = $1;
                                }
                            | CINT_DELIM_LPAREN ND_EXPRESSION CINT_DELIM_RPAREN
                                {
                                    $$ = $2;
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
    using namespace cint;
    using namespace std;
    for (auto &i : gpCmdSeq->cmds)
    {
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
            cout << reinterpret_cast<ternaryOperation *>(i.opr)->vars[0] << " | "
                 << reinterpret_cast<ternaryOperation *>(i.opr)->vars[1] << " | "
                 << reinterpret_cast<ternaryOperation *>(i.opr)->vars[2] << endl;
            break;
        
        default:
            cout << "unknown: " << endl;
        }
    }
    return 1;
}
