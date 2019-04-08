/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20180609

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "src/grammar.y"

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
#line 67 "src/grammar.cc"

#if ! defined(YYSTYPE) && ! defined(YYSTYPE_IS_DECLARED)
/* Default: YYSTYPE is the semantic value type. */
typedef int YYSTYPE;
# define YYSTYPE_IS_DECLARED 1
#endif

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

#if !(defined(yylex) || defined(YYSTATE))
int YYLEX_DECL();
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define CINT_TYPE_INT 257
#define CINT_TYPE_DOUBLE 258
#define CINT_TYPE_VOID 259
#define CINT_CTRL_IF 260
#define CINT_CTRL_WHILE 261
#define CINT_CTRL_BREAK 262
#define CINT_CTRL_CONTINUE 263
#define CINT_CTRL_RETURN 264
#define CINT_OPR_ADD 265
#define CINT_OPR_SUB 266
#define CINT_OPR_MUL 267
#define CINT_OPR_DIV 268
#define CINT_OPR_MOD 269
#define CINT_OPR_LOGIC_AND 270
#define CINT_OPR_LOGIC_OR 271
#define CINT_OPR_LT 272
#define CINT_OPR_EQ 273
#define CINT_OPR_GT 274
#define CINT_OPR_NE 275
#define CINT_OPR_LE 276
#define CINT_OPR_GE 277
#define CINT_OPR_ASSIGN 278
#define CINT_DELIM_LPAREN 279
#define CINT_DELIM_RPAREN 280
#define CINT_DELIM_LBRACE 281
#define CINT_DELIM_RBRACE 282
#define CINT_DELIM_LBRACKET 283
#define CINT_DELIM_RBRACKET 284
#define CINT_DELIM_SEMICOLON 285
#define CINT_DELIM_COMMA 286
#define CINT_IDENTIFIER 287
#define CINT_INTEGER 288
#define YYERRCODE 256
typedef short YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    0,    3,    1,    2,    2,    4,    4,    4,    5,
    8,    8,    8,    7,    7,    7,    6,   10,   11,   12,
   12,   13,   13,   14,   15,   16,   17,   17,   17,   18,
   18,   18,   18,   18,   19,   20,   20,   20,   21,   21,
   21,   21,   22,   23,   23,    9,
};
static const YYINT yylen[] = {                            2,
    0,    2,    0,    4,    0,    2,    1,    1,    1,    3,
    2,    2,    2,    1,    1,    1,    4,    1,    1,    1,
    3,    1,    3,    1,    1,    1,    1,    3,    3,    1,
    3,    3,    3,    3,    1,    1,    3,    3,    1,    3,
    3,    3,    1,    1,    3,    1,
};
static const YYINT yydefred[] = {                         1,
    0,    3,    2,    5,    0,    0,    0,    0,   14,   15,
   16,    4,   46,    6,    7,    8,    9,    0,    0,   11,
   12,   13,    0,    0,   10,    0,   44,    0,   18,    0,
    0,   22,   24,   25,    0,    0,   30,    0,    0,   39,
   43,    0,   17,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   45,    0,   23,    0,
    0,   31,   33,   32,   34,    0,    0,   40,   41,   42,
};
static const YYINT yydgoto[] = {                          1,
    3,    5,    4,   14,   15,   16,   17,   18,   27,   28,
   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
   39,   40,   41,
};
static const YYINT yysindex[] = {                         0,
 -236,    0,    0,    0, -257, -261, -245, -202,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -196, -193,    0,
    0,    0, -192, -259,    0, -259,    0, -179,    0, -162,
 -159,    0,    0,    0, -198, -255,    0, -210, -153,    0,
    0, -160,    0, -259, -259, -259, -259, -259, -259, -259,
 -259, -259, -259, -259, -259, -259,    0, -159,    0, -255,
 -255,    0,    0,    0,    0, -153, -153,    0,    0,    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -241,
 -173,    0,    0,    0, -175, -199,    0, -207, -262,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -172,    0, -191,
 -183,    0,    0,    0,    0, -239, -223,    0,    0,    0,
};
static const YYINT yygindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,   11,   95,
    0,    0,   78,   79,    0,    0,    0,   54,   10,    0,
   51,   63,    0,
};
#define YYTABLESIZE 124
static const YYINT yytable[] = {                          6,
    7,    8,   36,   36,    9,   10,   11,   36,   36,   36,
   36,   36,   36,   36,   36,   19,   48,   36,   49,   26,
   50,   51,   36,   20,   12,   37,   37,   13,   23,   13,
   37,   37,   37,   37,   37,   37,   37,   37,   19,   21,
   37,   38,   38,   19,    2,   37,   38,   38,   38,   38,
   38,   38,   38,   38,   52,   53,   38,   62,   63,   64,
   65,   38,   35,   35,   35,   35,   35,   35,   35,   35,
   27,   27,   35,   27,   46,   27,   47,   35,   28,   28,
   27,   28,   22,   28,   24,   27,   29,   29,   28,   29,
   13,   29,   25,   28,   26,   26,   29,   20,   21,   60,
   61,   29,   66,   67,   26,   43,   20,   21,   44,   26,
   45,   20,   21,   54,   55,   56,   68,   69,   70,   57,
   42,   58,    0,   59,
};
static const YYINT yycheck[] = {                        257,
  258,  259,  265,  266,  262,  263,  264,  270,  271,  272,
  273,  274,  275,  276,  277,    5,  272,  280,  274,  279,
  276,  277,  285,  285,  282,  265,  266,  287,   18,  287,
  270,  271,  272,  273,  274,  275,  276,  277,  280,  285,
  280,  265,  266,  285,  281,  285,  270,  271,  272,  273,
  274,  275,  276,  277,  265,  266,  280,   48,   49,   50,
   51,  285,  270,  271,  272,  273,  274,  275,  276,  277,
  270,  271,  280,  273,  273,  275,  275,  285,  270,  271,
  280,  273,  285,  275,  278,  285,  270,  271,  280,  273,
  287,  275,  285,  285,  270,  271,  280,  271,  271,   46,
   47,  285,   52,   53,  280,  285,  280,  280,  271,  285,
  270,  285,  285,  267,  268,  269,   54,   55,   56,  280,
   26,   44,   -1,   45,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 288
#define YYUNDFTOKEN 314
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"CINT_TYPE_INT",
"CINT_TYPE_DOUBLE","CINT_TYPE_VOID","CINT_CTRL_IF","CINT_CTRL_WHILE",
"CINT_CTRL_BREAK","CINT_CTRL_CONTINUE","CINT_CTRL_RETURN","CINT_OPR_ADD",
"CINT_OPR_SUB","CINT_OPR_MUL","CINT_OPR_DIV","CINT_OPR_MOD",
"CINT_OPR_LOGIC_AND","CINT_OPR_LOGIC_OR","CINT_OPR_LT","CINT_OPR_EQ",
"CINT_OPR_GT","CINT_OPR_NE","CINT_OPR_LE","CINT_OPR_GE","CINT_OPR_ASSIGN",
"CINT_DELIM_LPAREN","CINT_DELIM_RPAREN","CINT_DELIM_LBRACE","CINT_DELIM_RBRACE",
"CINT_DELIM_LBRACKET","CINT_DELIM_RBRACKET","CINT_DELIM_SEMICOLON",
"CINT_DELIM_COMMA","CINT_IDENTIFIER","CINT_INTEGER",0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,"illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : start",
"start :",
"start : start ND_BLOCK_STATEMENTS",
"$$1 :",
"ND_BLOCK_STATEMENTS : CINT_DELIM_LBRACE $$1 ND_STATEMENT_SEQUENCE CINT_DELIM_RBRACE",
"ND_STATEMENT_SEQUENCE :",
"ND_STATEMENT_SEQUENCE : ND_STATEMENT_SEQUENCE ND_STATEMENT",
"ND_STATEMENT : ND_DECLARE_VARIABLE",
"ND_STATEMENT : ND_ARITHMIC_OPERATION",
"ND_STATEMENT : ND_CONTROL_COMMAND",
"ND_DECLARE_VARIABLE : ND_TYPE_NAME ND_IDENTIFIER CINT_DELIM_SEMICOLON",
"ND_TYPE_NAME : CINT_TYPE_INT CINT_DELIM_SEMICOLON",
"ND_TYPE_NAME : CINT_TYPE_DOUBLE CINT_DELIM_SEMICOLON",
"ND_TYPE_NAME : CINT_TYPE_VOID CINT_DELIM_SEMICOLON",
"ND_CONTROL_COMMAND : CINT_CTRL_BREAK",
"ND_CONTROL_COMMAND : CINT_CTRL_CONTINUE",
"ND_CONTROL_COMMAND : CINT_CTRL_RETURN",
"ND_ARITHMIC_OPERATION : ND_IDENTIFIER CINT_OPR_ASSIGN ND_EXPRESSION CINT_DELIM_SEMICOLON",
"ND_EXPRESSION : ND_EXPRESSION_13",
"ND_EXPRESSION_13 : ND_EXPRESSION_12",
"ND_EXPRESSION_12 : ND_EXPRESSION_11",
"ND_EXPRESSION_12 : ND_EXPRESSION_12 CINT_OPR_LOGIC_OR ND_EXPRESSION_11",
"ND_EXPRESSION_11 : ND_EXPRESSION_10",
"ND_EXPRESSION_11 : ND_EXPRESSION_11 CINT_OPR_LOGIC_AND ND_EXPRESSION_10",
"ND_EXPRESSION_10 : ND_EXPRESSION_9",
"ND_EXPRESSION_9 : ND_EXPRESSION_8",
"ND_EXPRESSION_8 : ND_EXPRESSION_7",
"ND_EXPRESSION_7 : ND_EXPRESSION_6",
"ND_EXPRESSION_7 : ND_EXPRESSION_7 CINT_OPR_EQ ND_EXPRESSION_6",
"ND_EXPRESSION_7 : ND_EXPRESSION_7 CINT_OPR_NE ND_EXPRESSION_6",
"ND_EXPRESSION_6 : ND_EXPRESSION_5",
"ND_EXPRESSION_6 : ND_EXPRESSION_6 CINT_OPR_LT ND_EXPRESSION_5",
"ND_EXPRESSION_6 : ND_EXPRESSION_6 CINT_OPR_LE ND_EXPRESSION_5",
"ND_EXPRESSION_6 : ND_EXPRESSION_6 CINT_OPR_GT ND_EXPRESSION_5",
"ND_EXPRESSION_6 : ND_EXPRESSION_6 CINT_OPR_GE ND_EXPRESSION_5",
"ND_EXPRESSION_5 : ND_EXPRESSION_4",
"ND_EXPRESSION_4 : ND_EXPRESSION_3",
"ND_EXPRESSION_4 : ND_EXPRESSION_4 CINT_OPR_ADD ND_EXPRESSION_3",
"ND_EXPRESSION_4 : ND_EXPRESSION_4 CINT_OPR_SUB ND_EXPRESSION_3",
"ND_EXPRESSION_3 : ND_EXPRESSION_2",
"ND_EXPRESSION_3 : ND_EXPRESSION_3 CINT_OPR_MUL ND_EXPRESSION_2",
"ND_EXPRESSION_3 : ND_EXPRESSION_3 CINT_OPR_DIV ND_EXPRESSION_2",
"ND_EXPRESSION_3 : ND_EXPRESSION_3 CINT_OPR_MOD ND_EXPRESSION_2",
"ND_EXPRESSION_2 : ND_EXPRESSION_1",
"ND_EXPRESSION_1 : ND_IDENTIFIER",
"ND_EXPRESSION_1 : CINT_DELIM_LPAREN ND_EXPRESSION CINT_DELIM_RPAREN",
"ND_IDENTIFIER : CINT_IDENTIFIER",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;
int      yynerrs;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 291 "src/grammar.y"

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
#line 425 "src/grammar.cc"

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yym = 0;
    yyn = 0;
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        yychar = YYLEX;
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;

    YYERROR_CALL("syntax error");

    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);

    switch (yyn)
    {
case 3:
#line 86 "src/grammar.y"
	{
                                    gpCmdSeq = new cint::cmdSeq;
                                    gpCmdSeq->type = cint::cmdSeqType::normal;
                                    gpExistTmpVar = new std::unordered_set<std::string>;
                                }
break;
case 17:
#line 127 "src/grammar.y"
	{
                                    using namespace cint;
                                    gpCmdSeq->cmds.emplace_back(cmdType::binaryOperation,
                                                                std::unique_ptr<binaryOperation>
                                                                (new binaryOperation
                                                                {
                                                                    binaryOprType::assign,
                                                                    *reinterpret_cast<std::string *>(yystack.l_mark[-3].data),
                                                                    *reinterpret_cast<std::string *>(yystack.l_mark[-1].data)
                                                                }));
                                }
break;
case 18:
#line 143 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 19:
#line 149 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 20:
#line 155 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 21:
#line 159 "src/grammar.y"
	{
                                    TERNARY_ARITHMIC_OPERATION(assignLogicOr, yyval, yystack.l_mark[-2], yystack.l_mark[0]);
                                }
break;
case 22:
#line 165 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 23:
#line 169 "src/grammar.y"
	{
                                    TERNARY_ARITHMIC_OPERATION(assignLogicAnd, yyval, yystack.l_mark[-2], yystack.l_mark[0]);
                                }
break;
case 24:
#line 175 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 25:
#line 181 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 26:
#line 187 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 27:
#line 193 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 28:
#line 197 "src/grammar.y"
	{
                                    TERNARY_ARITHMIC_OPERATION(assignEqual, yyval, yystack.l_mark[-2], yystack.l_mark[0]);
                                }
break;
case 29:
#line 201 "src/grammar.y"
	{
                                    TERNARY_ARITHMIC_OPERATION(assignNotEqual, yyval, yystack.l_mark[-2], yystack.l_mark[0]);
                                }
break;
case 30:
#line 207 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 31:
#line 211 "src/grammar.y"
	{
                                    TERNARY_ARITHMIC_OPERATION(assignLess, yyval, yystack.l_mark[-2], yystack.l_mark[0]);
                                }
break;
case 32:
#line 215 "src/grammar.y"
	{
                                    TERNARY_ARITHMIC_OPERATION(assignLessEqual, yyval, yystack.l_mark[-2], yystack.l_mark[0]);
                                }
break;
case 33:
#line 219 "src/grammar.y"
	{
                                    TERNARY_ARITHMIC_OPERATION(assignGreater, yyval, yystack.l_mark[-2], yystack.l_mark[0]);
                                }
break;
case 34:
#line 223 "src/grammar.y"
	{
                                    TERNARY_ARITHMIC_OPERATION(assignGreaterEqual, yyval, yystack.l_mark[-2], yystack.l_mark[0]);
                                }
break;
case 35:
#line 229 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 36:
#line 235 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 37:
#line 239 "src/grammar.y"
	{
                                    TERNARY_ARITHMIC_OPERATION(assignSum, yyval, yystack.l_mark[-2], yystack.l_mark[0]);
                                }
break;
case 38:
#line 243 "src/grammar.y"
	{
                                    TERNARY_ARITHMIC_OPERATION(assignDifference, yyval, yystack.l_mark[-2], yystack.l_mark[0]);
                                }
break;
case 39:
#line 249 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 40:
#line 253 "src/grammar.y"
	{
                                    TERNARY_ARITHMIC_OPERATION(assignProduct, yyval, yystack.l_mark[-2], yystack.l_mark[0]);
                                }
break;
case 41:
#line 257 "src/grammar.y"
	{
                                    TERNARY_ARITHMIC_OPERATION(assignQuotient, yyval, yystack.l_mark[-2], yystack.l_mark[0]);
                                }
break;
case 42:
#line 261 "src/grammar.y"
	{
                                    TERNARY_ARITHMIC_OPERATION(assignResidue, yyval, yystack.l_mark[-2], yystack.l_mark[0]);
                                }
break;
case 43:
#line 267 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 44:
#line 273 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[0];
                                }
break;
case 45:
#line 277 "src/grammar.y"
	{
                                    yyval = yystack.l_mark[-1];
                                }
break;
case 46:
#line 283 "src/grammar.y"
	{
                                    using namespace cint;
                                    yyval = yaccInfo(yaccInfo::infoType::varName,
                                                  gLexIdentifier);
                                }
break;
#line 822 "src/grammar.cc"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            yychar = YYLEX;
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    YYERROR_CALL("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
