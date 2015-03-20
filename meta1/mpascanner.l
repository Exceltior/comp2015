%{
#include <stdio.h>
static int col = 1;
static int auxLine;
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

"{"|"(*"                                            {BEGIN(COMMENT); auxLine = yylineno; col += yyleng;}
<COMMENT>"}"|"*)"                                   {BEGIN(0); col += yyleng;}
<COMMENT>.                                          {col += yyleng;}
<COMMENT>\n                                         {;}
<COMMENT><<EOF>>                                    {BEGIN(0); {printf("Line %d, col %d: unterminated comment\n", auxLine, col);}}

{int}                                               {printf("INTLIT(%s)\n", yytext); col+=yyleng; }
{reallit}   					                    {printf("REALLIT(%s)\n", yytext); col+=yyleng; }
{string}                                            {yytext[strlen(yytext)-1] = '\0';
                                                     printf("STRING('%s')\n", yytext+1); col += yyleng;}
'(''|[^'\n])*                                       {printf("Line %d, col %d: unterminated string\n", yylineno, col);}

":="                                                {printf("ASSIGN\n"); col+=yyleng;}
"begin"                                             {printf("BEGIN\n"); col+=yyleng;}
":"                                                 {printf("COLON\n"); col+=yyleng;}
","                                                 {printf("COMMA\n"); col+=yyleng;}
"do"                                                {printf("DO\n"); col+=yyleng;}
"."                                                 {printf("DOT\n"); col+=yyleng;}
"else"                                              {printf("ELSE\n"); col+=yyleng;}
"end"                                               {printf("END\n"); col+=yyleng;}
"forward"                                           {printf("FORWARD\n"); col+=yyleng;}
"function"                                          {printf("FUNCTION\n"); col+=yyleng;}
"if"                                                {printf("IF\n"); col+=yyleng;}
"("                                                 {printf("LBRAC\n"); col+=yyleng;}
"not"                                               {printf("NOT\n"); col+=yyleng;}
"output"                                            {printf("OUTPUT\n"); col+=yyleng;}
"paramstr"                                          {printf("PARAMSTR\n"); col+=yyleng;}
"program"                                           {printf("PROGRAM\n"); col+=yyleng;}
")"                                                 {printf("RBRAC\n"); col+=yyleng;}
"repeat"                                            {printf("REPEAT\n"); col+=yyleng;}
";"                                                 {printf("SEMIC\n"); col+=yyleng;}
"then"                                              {printf("THEN\n"); col+=yyleng;}
"until"                                             {printf("UNTIL\n"); col+=yyleng;}
"val"                                               {printf("VAL\n"); col+=yyleng;}
"var"                                               {printf("VAR\n"); col+=yyleng;}
"while"                                             {printf("WHILE\n"); col+=yyleng;}
"writeln"                                           {printf("WRITELN\n"); col+=yyleng;}
"and"|"or"                                          {printf("OP1(%s)\n", yytext); col+=yyleng;}
"<"|">"|"="|"<>"|"<="|">="                          {printf("OP2(%s)\n", yytext); col+=yyleng;}
"+"|"-"                                             {printf("OP3(%s)\n", yytext); col+=yyleng;}
"*"|"/"|"mod"|"div"                                 {printf("OP4(%s)\n", yytext); col+=yyleng;}

"abs"|"arctan"|"array"|"case"|"char"|"chr"|"const"|"cos"|"dispose"|"downto"|"eof"|"eoln"|"exp"|"file"|"for"|"get"|"goto"|"in"|"input"|"label"|"ln"|"maxint"|"new"|"nil"|"odd"|"of"|"ord"|"pack"|"packed"|"page"|"pred"|"procedure"|"put"|"read"|"readln"|"record"|"reset"|"rewrite"|"round"|"set"|"sqr"|"sin"|"sqrt"|"succ"|"text"|"to"|"trunc"|"type"|"unpack"|"with"|"write"          {printf("RESERVED(%s)\n", yytext); col+=yyleng;}

[a-zA-Z][0-9a-zA-Z]*					            {printf("ID(%s)\n", yytext); col+=yyleng;}

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



