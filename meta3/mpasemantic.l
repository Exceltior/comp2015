%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

int col = 1;
static int auxLine, auxCol;

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; yylloc.first_column = col; yylloc.last_column = col+yyleng-1; col += yyleng;

%}

%option yylineno
%option case-insensitive
%x COMMENT

int                                                 [0-9]+
float                                               [0-9]+.[0-9]+
reallit						                        [0-9]+("."[0-9]+)?("e"[+-]?[0-9]+)?
whitespace                                          [ \t\v\f]*
string                                              '(''|[^'\n])*'

%%

"{"|"(*"                                            {BEGIN(COMMENT); auxLine = yylineno; auxCol = col; col += yyleng;}
<COMMENT>"}"|"*)"                                   {BEGIN(0); col += yyleng;}
<COMMENT>.                                          {col += yyleng;}
<COMMENT>\n                                         {col = 1;}
<COMMENT><<EOF>>                                    {BEGIN(0); {printf("Line %d, col %d: unterminated comment\n", auxLine, auxCol);}}

{int}                                               {col+=yyleng; yylval.intlit = (char*)strdup(yytext); return INTLIT;}
{reallit}   					                    {col+=yyleng; yylval.reallit = (char*)strdup(yytext); return REALLIT;}
{string}                                            {yylval.string = (char*)strdup(yytext); col += yyleng; return STRING;}
'(''|[^'\n])*                                       {printf("Line %d, col %d: unterminated string\n", yylineno, col);}

":="                                                {col+=yyleng; return ASSIGN;}
"begin"                                             {col+=yyleng; return YBEGIN;}
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
"and"                 	                            {col+=yyleng; return AND;}
"or"                                                {col+=yyleng; return OR;}
"<"|">"|"="|"+"|"-"|"*"|"/"           		        {col+=yyleng; return yytext[0];}
"<>"                                                {col+=yyleng; return DIF;}
"<="                                                {col+=yyleng; return LESSEQ;}
">="                                                {col+=yyleng; return GREATEQ;}
"mod"                                               {col+=yyleng; return MOD;}
"div"                                               {col+=yyleng; return DIV;}

"abs"|"arctan"|"array"|"case"|"char"|"chr"|"const"|"cos"|"dispose"|"downto"|"eof"|"eoln"|"exp"|"file"|"for"|"get"|"goto"|"in"|"input"|"label"|"ln"|"maxint"|"new"|"nil"|"odd"|"of"|"ord"|"pack"|"packed"|"page"|"pred"|"procedure"|"put"|"read"|"readln"|"record"|"reset"|"rewrite"|"round"|"set"|"sqr"|"sin"|"sqrt"|"succ"|"text"|"to"|"trunc"|"type"|"unpack"|"with"|"write"          {col+=yyleng; return RESERVED;}

[a-zA-Z][0-9a-zA-Z]*				                {col+=yyleng; yylval.id = (char*)strdup(yytext); return ID;}

\n                                                  {col = 1;}
{whitespace}                                        {col += yyleng;}

<<EOF>>						    {col += yyleng; return 0;}

.                                                   {printf("Line %d, col %d: illegal character ('%c')\n", yylineno, col, yytext[0]); col += yyleng;}

%%

int yywrap() {
    return 1;
}
