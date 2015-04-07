%{
#include<stdio.h>
%}

%token <intlit> INTLIT
%token <reallit> REALLIT
%token <string> STRING
%token <id> ID
%token <op1> OP1
%token <op2> OP2
%token <op3> OP3
%token <op4> OP4
%token ASSIGN RBRAC DOT REPEAT FUNCTION COMMA VAL END LBRAC WHILE YBEGIN OUTPUT PROGRAM ELSE SEMIC COLON PARAMSTR IF UNTIL DO THEN VAR FORWARD NOT WRITELN RESERVED

%union {
	int intlit;
	char* reallit, *string, *id, *op1, *op2, *op3, *op4;
}

%left OP3
%left OP4
%left OP1
%left OP2

%%

Prog: 			ProgHeading SEMIC ProgBlock DOT

ProgHeading: 		PROGRAM ID LBRAC OUTPUT RBRAC

ProgBlock: 		VarPart FuncPart StatPart

VarPart: 		VAR VarDeclaration SEMIC VarPartAux
		|	%empty

VarPartAux: 	VarDeclaration SEMIC VarPartAux
		|	%empty

VarDeclaration: 	IDList COLON ID

IDList: 		ID IDListAux

IDListAux:	COMMA ID IDListAux
		|	%empty

FuncPart:  FuncDeclaration SEMIC FuncPart
		|	%empty

FuncDeclaration: 	FuncHeading SEMIC FORWARD
		|	FuncIdent SEMIC FuncBlock

FuncDeclaration: 	FuncHeading SEMIC FuncBlock

FuncHeading: 		FUNCTION ID FuncHeadingAux COLON ID

FuncHeadingAux:		FormalParamList
		|	%empty

FuncIdent:		FUNCTION ID

FormalParamList: 	LBRAC FormalParams FormalParamListAux RBRAC

FormalParamListAux: 	SEMIC FormalParams | FormalParamListAux
		|	%empty

FormalParams:		FormalParamsAux IDList COLON ID

FormalParamsAux:	VAR
		|	%empty

FuncBlock: 		VarPart StatPart

StatPart: 		CompStat

CompStat:		YBEGIN StatList END

StatList: 		Stat SemicStatAux

SemicStatAux:	SEMIC Stat SemicStatAux
		|	%empty

Stat: 			CompStat
		|	IF Expr THEN Stat ELSE Stat
		|	IF Expr THEN Stat
		|	WHILE Expr DO Stat
		|	REPEAT StatList UNTIL Expr
		|	VAL LBRAC PARAMSTR LBRAC Expr RBRAC COMMA ID RBRAC
		|	ID ASSIGN Expr
		|	WRITELN WritelnPList
		|	WRITELN
		|	%empty

WritelnPList:		LBRAC Expr CommaExpStrAux RBRAC
		|	LBRAC STRING CommaExpStrAux RBRAC

CommaExpStrAux:		COMMA Expr
		|	COMMA STRING
		|	COMMA Expr CommaExpStrAux
		|	COMMA STRING CommaExpStrAux
		|	%empty

Expr:			Expr OP1 Expr
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

ParamList:		LBRAC Expr CommaExprAux RBRAC

CommaExprAux:	COMMA Expr CommaExprAux
		|	%empty


%%
int main()
{
	yyparse();
}
