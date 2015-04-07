%{
#include<stdio.h>
%}

%token <number> INTLIT
%token <str> REALLIT STRING ID OP1 OP2 OP3 OP4
%token ASSIGN RBRAC DOT REPEAT FUNCTION COMMA VAL END LBRAC WHILE YBEGIN OUTPUT PROGRAM ELSE SEMIC COLON PARAMSTR IF UNTIL DO THEN VAR FORWARD NOT WRITELN RESERVED

%union {
	int number;
	char* str;
}

%%
rekt:								{}

%%
int main()
{
	yyparse();
}
