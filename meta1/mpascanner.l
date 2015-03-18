%{
#include <stdio.h>
static int line = 1;
static int col = 1;
%}

%option caseless

int                                     [0-9]+
whitespace                              [ \t\v\f]*
string                                  '(''|[^\n])+'

%%
{int}                                   {printf("INTLIT(%s)\n", yytext);}
({int}"."{int})|({int}e[+-]?{int})      {printf("REALLIT(%s)\n", yytext);}
{string}                                {yytext[strlen(yytext)-1] = '\0';
                                         printf("STRING(%s)\n", yytext+1);}
'(''|[^\n])+                            {printf("Line %d, col %d: unterminated string\n", line, col);}

":="                                    {printf("ASSIGN\n");}
"begin"                                 {printf("BEGIN\n");}
":"                                     {printf("COLON\n");}
","                                     {printf("COMMA\n");}
"do"                                    {printf("DO\n");}
"."                                     {printf("DOT\n");}
"else"                                  {printf("ELSE\n");}
"end"                                   {printf("END\n");}
"forward"                               {printf("FORWARD\n");}
"function"                              {printf("FUNCTION\n");}
"if"                                    {printf("IF\n");}
"("                                     {printf("LBRAC\n");}
"not"                                   {printf("NOT\n");}
"output"                                {printf("OUTPUT\n");}
"paramstr"                              {printf("PARAMSTR\n");}
"program"                               {printf("PROGRAM\n");}
")"                                     {printf("RBRAC\n");}
"repeat"                                {printf("REPEAT\n");}
";"                                     {printf("SEMIC\n");}
"then"                                  {printf("THEN\n");}
"until"                                 {printf("UNTIL\n");}
"val"                                   {printf("VAL\n");}
"var"                                   {printf("VAR\n");}
"while"                                 {printf("WHILE\n");}
"writeln"                               {printf("WRITELN\n");}
"and"|"or"                              {printf("OP1(%s)\n", yytext);}
"<"|">"|"="|"<>"|"<="|">="              {printf("OP2(%s)\n", yytext);}
"+"|"-"                                 {printf("OP3(%s)\n", yytext);}
"*"|"/"|"mod"|"div"                     {printf("OP4(%s)\n", yytext);}

"abs"|"arctan"|"char"|"chr"|"cos"|"dispose"|"eof"|"eoln"|"exp"|"get"|"input"|"ln"|"maxint"|"new"|"odd"|"ord"|"output"|"pack"|"page"|"pred"|"put"|"read"|"readln"|"reset"|"rewrite"|"round"|"sin"|"sqr"|"sqrt"|"succ"|"text"|"trunc"|"unpack"|"write"          {printf("RESERVED(%s)\n", yytext);}

[a-zA-Z][0-9a-zA-Z]*					{printf("ID(%s)\n", yytext);}

\n                                      {col = 1; line++;}
{whitespace}                            {col += yyleng;}

.                                       {printf("Line %d, col %d: illegal character ('%c')\n", line, col, yytext);}

%%
int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
