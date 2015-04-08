%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#define OP_PARSING_TREE "-t"

typedef struct node {
	char* type;
	void* value;
	struct node** children;
} node;

node* parsing_tree;

node* new_node(char* type, void* value) {
	node* n = (node*)malloc(sizeof(node));
	n->type = type;
	n->value = value;
	return n;
}

node* create_node(char* type, int n_children, ...) {
	va_list args;
	va_start(args, n_children);
	register int i;

	node* parent = new_node(type, NULL);
	parent->children = (node**)malloc(sizeof(node)*n_children);

	for (i=0;i<n_children;i++) {
		parent->children[i] = va_arg(args, node*);
	}
	va_end(args);
	return parent;
}

void print_parsing_tree() {
	printf("PARSING TREE\n");
}
/*$$=create_node("Prog", 2, $1, $3);*/
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

%type <node*> Prog ProgHeading ProgBlock VarPart VarPartAux VarDeclaration IDList IDListAux FuncPart FuncDeclaration FuncHeading FuncHeadingAux FormalParamList FormalParamListAux FormalParams FormalParamsAux FuncBlock StatPart CompStat StatList SemicStatAux Stat WritelnPList CommaExpStrAux Expr ParamList CommaExprAux

%%

Prog:					ProgHeading SEMIC ProgBlock DOT						{$$=create_node("Prog", 2, $1, $3);}

ProgHeading: 			PROGRAM ID LBRAC OUTPUT RBRAC						{;}

ProgBlock: 				VarPart FuncPart StatPart							{;}

VarPart: 				VAR VarDeclaration SEMIC VarPartAux					{;}
		|				%empty												{;}

VarPartAux: 			VarDeclaration SEMIC VarPartAux						{;}
		|				%empty												{;}

VarDeclaration: 		IDList COLON ID										{;}

IDList: 				ID IDListAux										{;}

IDListAux:				COMMA ID IDListAux									{;}
		|				%empty												{;}

FuncPart:  				FuncDeclaration SEMIC FuncPart						{;}
		|				%empty												{;}

FuncDeclaration: 		FuncHeading SEMIC FORWARD							{;}
		|				FuncIdent SEMIC FuncBlock							{;}
		|				FuncHeading SEMIC FuncBlock							{;}

FuncHeading: 			FUNCTION ID FuncHeadingAux COLON ID					{;}

FuncHeadingAux:			FormalParamList										{;}
		|				%empty												{;}

FuncIdent:				FUNCTION ID											{;}

FormalParamList: 		LBRAC FormalParams FormalParamListAux RBRAC 		{;}

FormalParamListAux: 	SEMIC FormalParams FormalParamListAux				{;}
		|				%empty												{;}

FormalParams:			FormalParamsAux IDList COLON ID						{;}

FormalParamsAux:		VAR													{;}
		|				%empty												{;}

FuncBlock: 				VarPart StatPart									{;}

StatPart: 				CompStat											{;}

CompStat:				YBEGIN StatList END									{;}

StatList: 				Stat SemicStatAux									{;}

SemicStatAux:			SEMIC Stat SemicStatAux								{;}
		|				%empty												{;}

Stat: 					CompStat											{;}
		|				IF Expr THEN Stat ELSE Stat							{;}
		|				IF Expr THEN Stat									{;}
		|				WHILE Expr DO Stat									{;}
		|				REPEAT StatList UNTIL Expr							{;}
		|				VAL LBRAC PARAMSTR LBRAC Expr RBRAC COMMA ID RBRAC 	{;}
		|				ID ASSIGN Expr										{;}
		|				WRITELN WritelnPList								{;}
		|				WRITELN												{;}
		|				%empty												{;}

WritelnPList:			LBRAC Expr CommaExpStrAux RBRAC						{;}
		|				LBRAC STRING CommaExpStrAux RBRAC					{;}

CommaExpStrAux:			COMMA Expr CommaExpStrAux							{;}
		|				COMMA STRING CommaExpStrAux							{;}
		|				%empty												{;}

Expr:					Expr AND Expr										{;}
		|				Expr OR Expr										{;}
		|		 		Expr '<' Expr										{;}
		|				Expr '>' Expr										{;}
		|				Expr '=' Expr										{;}
		|				Expr DIF Expr										{;}
		|				Expr LESSEQ Expr									{;}
		|				Expr GREATEQ Expr									{;}
		|				Expr '+' Expr										{;}
		|				Expr '-' Expr										{;}
		|				Expr '*' Expr										{;}
		|		 		Expr MOD Expr										{;}
		|				Expr '/' Expr										{;}
		|				Expr DIV Expr										{;}
		|				'+' Expr											{;}
		|				'-' Expr											{;}
		|				NOT Expr											{;}
		|				LBRAC Expr RBRAC									{;}
		|				INTLIT												{;}
		|			 	REALLIT												{;}
		|				ID ParamList										{;}

		|				ID													{;}
ParamList:				LBRAC Expr CommaExprAux RBRAC						{;}

CommaExprAux:			COMMA Expr CommaExprAux								{;}
		|				%empty												{;}

%%
int main(int argc, char** argv) {
	yyparse();
	if (argc > 1) {
		if (!strcmp(argv[1], OP_PARSING_TREE)) {
			print_parsing_tree();
		}
	}
	return 0;
}
