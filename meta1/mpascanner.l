%{
#include <stdio.h>
static int col = 1;
%}

%option yylineno
%option case-insensitive
%x COMMENT

int                                                 [0-9]+
float                                               [0-9]+.[0-9]+
whitespace                                          [ \t\v\f]*
string                                              '(''|[^'\n])*'

%%

"{"|"(*"                                            {BEGIN(COMMENT);}
<COMMENT>"}"|"*)"                                   {BEGIN(0);}
<COMMENT>.                                          {;}
<COMMENT>\n                                         {col = 1;}
<COMMENT><<EOF>>                                    {BEGIN(0); {printf("Line %d, col %d: unterminated comment\n", yylineno-1, col);}}

{int}                                               {printf("INTLIT(%s)\n", yytext);}
{float}|(({int}|{float})"e"["+""-"]?({int}|{float}))    {printf("REALLIT(%s)\n", yytext);}
{string}                                            {yytext[strlen(yytext)-1] = '\0';
                                                     printf("STRING(%s)\n", yytext+1); col += yyleng;}
'(''|[^'\n])*                                        {printf("Line %d, col %d: unterminated string\n", yylineno, col);}

":="                                                {printf("ASSIGN\n");}
"begin"                                             {printf("BEGIN\n");}
":"                                                 {printf("COLON\n");}
","                                                 {printf("COMMA\n");}
"do"                                                {printf("DO\n");}
"."                                                 {printf("DOT\n");}
"else"                                              {printf("ELSE\n");}
"end"                                               {printf("END\n");}
"forward"                                           {printf("FORWARD\n");}
"function"                                          {printf("FUNCTION\n");}
"if"                                                {printf("IF\n");}
"("                                                 {printf("LBRAC\n");}
"not"                                               {printf("NOT\n");}
"output"                                            {printf("OUTPUT\n");}
"paramstr"                                          {printf("PARAMSTR\n");}
"program"                                           {printf("PROGRAM\n");}
")"                                                 {printf("RBRAC\n");}
"repeat"                                            {printf("REPEAT\n");}
";"                                                 {printf("SEMIC\n");}
"then"                                              {printf("THEN\n");}
"until"                                             {printf("UNTIL\n");}
"val"                                               {printf("VAL\n");}
"var"                                               {printf("VAR\n");}
"while"                                             {printf("WHILE\n");}
"writeln"                                           {printf("WRITELN\n");}
"and"|"or"                                          {printf("OP1(%s)\n", yytext);}
"<"|">"|"="|"<>"|"<="|">="                          {printf("OP2(%s)\n", yytext);}
"+"|"-"                                             {printf("OP3(%s)\n", yytext);}
"*"|"/"|"mod"|"div"                                 {printf("OP4(%s)\n", yytext);}

"abs"|"arctan"|"array"|"case"|"char"|"chr"|"const"|"cos"|"dispose"|"downto"|"eof"|"eoln"|"exp"|"file"|"for"|"get"|"goto"|"in"|"input"|"label"|"ln"|"maxint"|"new"|"nil"|"odd"|"of"|"ord"|"output"|"pack"|"packed"|"page"|"pred"|"procedure"|"put"|"read"|"readln"|"record"|"reset"|"rewrite"|"round"|"sin"|"sqr"|"sqrt"|"succ"|"text"|"to"|"trunc"|"type"|"unpack"|"with"|"write"          {printf("RESERVED(%s)\n", yytext);}

[a-zA-Z][0-9a-zA-Z]*					            {printf("ID(%s)\n", yytext);}

\n                                                  {col = 1;}
{whitespace}                                        {col += yyleng;}

.                                                   {printf("Line %d, col %d: illegal character ('%c')\n", yylineno, col, yytext[0]);}

%%
int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
