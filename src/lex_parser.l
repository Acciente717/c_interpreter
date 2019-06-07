
%%

 /* Reserved Type Keywords */
long      { return CINT_TYPE_LONG; }
double   { return CINT_TYPE_DOUBLE; }
void     { return CINT_TYPE_VOID; }

 /* Reserved Control Keywords */
if       { return CINT_CTRL_IF; }
else     { return CINT_CTRL_ELSE; }
while    { return CINT_CTRL_WHILE; }
break    { return CINT_CTRL_BREAK; }
continue { return CINT_CTRL_CONTINUE; }
return   { return CINT_CTRL_RETURN; }

 /* Operators */
"+"      { return CINT_OPR_ADD; }
"-"      { return CINT_OPR_SUB; }
"*"      { return CINT_OPR_MUL; }
"/"      { return CINT_OPR_DIV; }
"%"      { return CINT_OPR_MOD; }
"&&"     { return CINT_OPR_LOGIC_AND; }
"||"     { return CINT_OPR_LOGIC_OR; }
"<"      { return CINT_OPR_LT; }
"<="     { return CINT_OPR_LE; }
"=="     { return CINT_OPR_EQ; }
">"      { return CINT_OPR_GT; }
">="     { return CINT_OPR_GE; }
"!="     { return CINT_OPR_NE; }
"="      { return CINT_OPR_ASSIGN; }
"!"      { return CINT_OPR_NOT; }

 /* Delimiters */
"("      { return CINT_DELIM_LPAREN; }
")"      { return CINT_DELIM_RPAREN; }
"{"      { return CINT_DELIM_LBRACE; }
"}"      { return CINT_DELIM_RBRACE; }
"["      { return CINT_DELIM_LBRACKET; }
"]"      { return CINT_DELIM_RBRACKET; }
";"      { return CINT_DELIM_SEMICOLON; }
","      { return CINT_DELIM_COMMA; }

 /* Identifier */
[a-zA-Z_][a-zA-Z0-9_]* {
    gLexIdentifier = yytext;
    return CINT_IDENTIFIER;
}

 /* Integer */
[0-9]+ {
    gLexInteger = yytext;
    return CINT_INTEGER;
}
"0"[Xx][0-9]+ {
    gLexInteger = yytext;
    return CINT_INTEGER;
}

 /* Double */
[0-9]+\.[0-9]+ {
    gLexDouble = yytext;
    return CINT_DOUBLE;
}

 /* Blackhole for Whilespaces */
[ \t\r\n] ;

%%
