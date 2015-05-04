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

"{"|"(*"                                            {BEGIN(COMMENT); auxLine = yylineno; auxCol = col;}
<COMMENT>"}"|"*)"                                   {BEGIN(0);}
<COMMENT>.                                          {;}
<COMMENT>\n                                         {col = 1;}
<COMMENT><<EOF>>                                    {BEGIN(0); {printf("Line %d, col %d: unterminated comment\n", auxLine, auxCol);}}

{int}                                               {col+=yyleng; yylval.intlit = (char*)strdup(yytext); return INTLIT;}
{reallit}   					                    {col+=yyleng; yylval.reallit = (char*)strdup(yytext); return REALLIT;}
{string}                                            {yylval.string = (char*)strdup(yytext); return STRING;}
'(''|[^'\n])*                                       {printf("Line %d, col %d: unterminated string\n", yylineno, col);}

":="                                                {return ASSIGN;}
"begin"                                             {return YBEGIN;}
":"                                                 {return COLON;}
","                                                 {return COMMA;}
"do"                                                {return DO;}
"."                                                 {return DOT;}
"else"                                              {return ELSE;}
"end"                                               {return END;}
"forward"                                           {return FORWARD;}
"function"                                          {return FUNCTION;}
"if"                                                {return IF;}
"("                                                 {return LBRAC;}
"not"                                               {return NOT;}
"output"                                            {return OUTPUT;}
"paramstr"                                          {return PARAMSTR;}
"program"                                           {return PROGRAM;}
")"                                                 {return RBRAC;}
"repeat"                                            {return REPEAT;}
";"                                                 {return SEMIC;}
"then"                                              {return THEN;}
"until"                                             {return UNTIL;}
"val"                                               {return VAL;}
"var"                                               {return VAR;}
"while"                                             {return WHILE;}
"writeln"                                           {return WRITELN;}
"and"                 	                            {return AND;}
"or"                                                {return OR;}
"<"|">"|"="|"+"|"-"|"*"|"/"           		        {return yytext[0];}
"<>"                                                {return DIF;}
"<="                                                {return LESSEQ;}
">="                                                {return GREATEQ;}
"mod"                                               {return MOD;}
"div"                                               {return DIV;}

"abs"|"arctan"|"array"|"case"|"char"|"chr"|"const"|"cos"|"dispose"|"downto"|"eof"|"eoln"|"exp"|"file"|"for"|"get"|"goto"|"in"|"input"|"label"|"ln"|"maxint"|"new"|"nil"|"odd"|"of"|"ord"|"pack"|"packed"|"page"|"pred"|"procedure"|"put"|"read"|"readln"|"record"|"reset"|"rewrite"|"round"|"set"|"sqr"|"sin"|"sqrt"|"succ"|"text"|"to"|"trunc"|"type"|"unpack"|"with"|"write"          {return RESERVED;}

[a-zA-Z][0-9a-zA-Z]*				                {yylval.id = (char*)strdup(yytext); return ID;}

\n                                                  {col = 1;}
{whitespace}                                        {;}

<<EOF>>						    {return 0;}

.                                                   {printf("Line %d, col %d: illegal character ('%c')\n", yylineno, col, yytext[0]);}

%%

int yywrap() {
    return 1;
}
