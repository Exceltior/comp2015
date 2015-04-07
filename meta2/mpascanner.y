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
Prog: 				ProgHeading SEMIC ProgBlock DOT

ProgHeading: 		PROGRAM ID LBRAC OUTPUT RBRAC

ProgBlock: 			VarPart FuncPart StatPart

VarPart: 			VAR VarDeclaration SEMIC VarPartAux
		|			%empty

VarPartAux:			VarDeclaration SEMIC
		|			VarDeclaration SEMIC VarPartAux
		|			%empty

VarDeclaration: 	IDList COLON ID

IDList: 			ID IDListAux

IDListAux:			COMMA ID
		|			COMMA ID IDListAux
		|			%empty

FuncPart: 			FuncDeclaration SEMIC
		|			FuncDeclaration SEMIC FuncPart
		|			%empty

FuncDeclaration: 	FuncHeading SEMIC FORWARD
		|			FuncIdent SEMIC FuncBlock

FuncDeclaration: 	FuncHeading SEMIC FuncBlock

FuncHeading: 		FUNCTION ID FuncHeadingAux COLON ID

FuncHeadingAux:		FormalParamList
		|			%empty

FuncIdent:			FUNCTION ID

FormalParamList: 	LBRAC FormalParams FormalParamListAux RBRAC

FormalParamListAux: SEMIC FormalParams
		|			SEMIC FormalParams | FormalParamListAux
		|			%empty

FormalParams:		FormalParamsAux IDList COLON ID

FormalParamsAux:	VAR
		|			%empty

FuncBlock: 			VarPart StatPart

StatPart: 			CompStat

%%
int main()
{
	yyparse();
}
