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
CompStat:	YBEGIN StatList END

StatList: 	Stat SemicStatAux

SemicStatAux:	SEMIC Stat
	|	SEMIC Stat asd
	|	%empty

Stat: 		CompStat
	|	IF Expr THEN Stat ELSE Stat
	|	IF Expr THEN Stat
	|	WHILE Expr DO Stat
	|	REPEAT StatList UNTIL Expr
	|	VAL LBRAC PARAMSTR LBRAC Expr RBRAC COMMA ID RBRAC
	|	ID ASSIGN Expr
	|	WRITELN WritelnPList
	|	WRITELN
	|	%empty

WritelnPList:	LBRAC Expr CommaExpStrAux RBRAC
	|	LBRAC STRING CommaExpStrAux RBRAC

CommaExpStrAux:	COMMA Expr
	|	COMMA STRING
	|	COMMA Expr CommaExpStrAux
	|	COMMA STRING CommaExprStrAux
	|	%empty

Expr:		Expr OP1 Expr 
	| 	Expr OP2 Expr
	| 	Expr OP3 Expr 
	| 	Expr OP4 Expr
	|	OP3 Expr
	|	NOT Expr
	|	LBRAC Expr RBRAC
	|	INTLIT 
	| 	REALLIT
	|	ID ParamList
	|	ID
	
ParamList:	LBRAC Expr CommaExprAux RBRAC

CommaExprAux:	COMMA Expr
	|	COMMA Expr CommaExprAux
	|	%empty

%%
int main()
{
	yyparse();
}
