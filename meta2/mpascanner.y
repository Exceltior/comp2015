%{
#include<stdio.h>
%}

%token <intlit> INTLIT
%token <reallit> REALLIT
%token <string> STRING
%token <id> ID
%token ASSIGN RBRAC DOT REPEAT FUNCTION COMMA VAL END LBRAC WHILE YBEGIN OUTPUT PROGRAM ELSE SEMIC COLON PARAMSTR IF UNTIL DO THEN VAR FORWARD NOT WRITELN RESERVED
%token AND OR MOD DIV DIF LESSEQ GREATEQ

%union {
	int intlit;
	char* reallit, *string, *id;
}

%right ELSE THEN
%right ASSIGN
%left '=' '<' '>' DIF LESSEQ GREATEQ
%left '+' '-' OR
%left '*' '/' DIV MOD AND
%left NOT

%%

Prog: 			ProgHeading SEMIC ProgBlock DOT

ProgHeading: 		PROGRAM ID LBRAC OUTPUT RBRAC

ProgBlock: 		VarPart FuncPart StatPart

VarPart: 		VAR VarDeclaration SEMIC VarPartAux
		|	%empty

VarPartAux: 		VarDeclaration SEMIC VarPartAux
		|	%empty

VarDeclaration: 	IDList COLON ID

IDList: 		ID IDListAux

IDListAux:		COMMA ID IDListAux
		|	%empty

FuncPart:  		FuncDeclaration SEMIC FuncPart
		|	%empty

FuncDeclaration: 	FuncHeading SEMIC FORWARD
		|	FuncIdent SEMIC FuncBlock

FuncDeclaration: 	FuncHeading SEMIC FuncBlock 

FuncHeading: 		FUNCTION ID FuncHeadingAux COLON ID

FuncHeadingAux:		FormalParamList
		|	%empty

FuncIdent:		FUNCTION ID

FormalParamList: 	LBRAC FormalParams FormalParamListAux RBRAC

FormalParamListAux: 	SEMIC FormalParams FormalParamListAux
		|	%empty

FormalParams:		FormalParamsAux IDList COLON ID

FormalParamsAux:	VAR
		|	%empty

FuncBlock: 		VarPart StatPart

StatPart: 		CompStat

CompStat:		YBEGIN StatList END

StatList: 		Stat SemicStatAux

SemicStatAux:		SEMIC Stat SemicStatAux
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

CommaExpStrAux:		COMMA Expr CommaExpStrAux
		|	COMMA STRING CommaExpStrAux
		|	%empty

Expr:			Expr AND Expr
		|	Expr OR Expr
		| 	Expr '<' Expr
		|	Expr '>' Expr
		|	Expr '=' Expr
		|	Expr DIF Expr
		|	Expr LESSEQ Expr
		|	Expr GREATEQ Expr
		|	Expr '+' Expr
		|	Expr '-' Expr
		|	Expr '*' Expr
		|	Expr '/' Expr
		| 	Expr MOD Expr
		|	Expr DIV Expr
		|	'+' Expr
		|	'-' Expr
		|	NOT Expr
		|	LBRAC Expr RBRAC
		|	INTLIT
		| 	REALLIT
		|	ID ParamList
		|	ID

ParamList:		LBRAC Expr CommaExprAux RBRAC

CommaExprAux:		COMMA Expr CommaExprAux
		|	%empty


%%
int main()
{
	yyparse();
}
