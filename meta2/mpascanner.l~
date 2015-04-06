%{
#include <stdio.h>
static int col = 1;
static int auxLine, auxCol;
%}

%option yylineno
%option case-insensitive
%x COMMENT

int                                                 [0-9]+
float                                               [0-9]+.[0-9]+
reallit						    [0-9]+("."[0-9]+)?("e"[+-]?[0-9]+)?
whitespace                                          [ \t\v\f]*
string                                              '(''|[^'\n])*'

%%

"{"|"(*"                                            {BEGIN(COMMENT); auxLine = yylineno; auxCol = col; col += yyleng;}
<COMMENT>"}"|"*)"                                   {BEGIN(0); col += yyleng;}
<COMMENT>.                                          {col += yyleng;}
<COMMENT>\n                                         {col = 1;}
<COMMENT><<EOF>>                                    {BEGIN(0); {printf("Line %d, col %d: unterminated comment\n", auxLine, auxCol);}}

{int}                                               {col+=yyleng; return INTLIT;}
{reallit}   					    {col+=yyleng; return REALLIT;}
{string}                                            {yytext[strlen(yytext)-1] = '\0';
                                                     /*printf("STRING('%s')\n", yytext+1);*/ col += yyleng; return STRING;}
'(''|[^'\n])*                                       {printf("Line %d, col %d: unterminated string\n", yylineno, col);}

":="                                                {col+=yyleng; return ASSIGN;}
"begin"                                             {col+=yyleng; return BEGIN;}
":"                                                 {col+=yyleng; return COLON;}
","                                                 {col+=yyleng; return COMMA;}
"do"                                                {col+=yyleng; return DO;}
"."                                                 {col+=yyleng; return DOT;}
"else"                                              {col+=yyleng; return ELSE;}
"end"                                               {col+=yyleng; return END;}
"forward"                                           {col+=yyleng; return FORWARD;}
"function"                                          {col+=yyleng; return FUNCTION;}
"if"                                                {col+=yyleng; return IF;}
"("                                                 {col+=yyleng; return LBRAC;}
"not"                                               {col+=yyleng; return NOT;}
"output"                                            {col+=yyleng; return OUTPUT;}
"paramstr"                                          {col+=yyleng; return PARAMSTR;}
"program"                                           {col+=yyleng; return PROGRAM;}
")"                                                 {col+=yyleng; return RBRAC;}
"repeat"                                            {col+=yyleng; return REPEAT;}
";"                                                 {col+=yyleng; return SEMIC;}
"then"                                              {col+=yyleng; return THEN;}
"until"                                             {col+=yyleng; return UNTIL;}
"val"                                               {col+=yyleng; return VAL;}
"var"                                               {col+=yyleng; return VAR;}
"while"                                             {col+=yyleng; return WHILE;}
"writeln"                                           {col+=yyleng; return WRITELN;}
"and"|"or"                                          {col+=yyleng; return OP1;}
"<"|">"|"="|"<>"|"<="|">="                          {col+=yyleng; return OP2;}
"+"|"-"                                             {col+=yyleng; return OP3;}
"*"|"/"|"mod"|"div"                                 {col+=yyleng; return OP4;}

"abs"|"arctan"|"array"|"case"|"char"|"chr"|"const"|"cos"|"dispose"|"downto"|"eof"|"eoln"|"exp"|"file"|"for"|"get"|"goto"|"in"|"input"|"label"|"ln"|"maxint"|"new"|"nil"|"odd"|"of"|"ord"|"pack"|"packed"|"page"|"pred"|"procedure"|"put"|"read"|"readln"|"record"|"reset"|"rewrite"|"round"|"set"|"sqr"|"sin"|"sqrt"|"succ"|"text"|"to"|"trunc"|"type"|"unpack"|"with"|"write"          {col+=yyleng; return RESERVED;}

[a-zA-Z][0-9a-zA-Z]*				    {col+=yyleng; return ID;}

\n                                                  {col = 1;}
{whitespace}                                        {col += yyleng;}

.                                                   {printf("Line %d, col %d: illegal character ('%c')\n", yylineno, col, yytext[0]); col += yyleng;}

%%
int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}



