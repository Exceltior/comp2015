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

node* create_terminal(char* type, void* value) {
	return new_node(type, value);
}

void print_parsing_tree() {
	printf("PARSING TREE\n");
}
%}

%token <intlit> INTLIT
%token <reallit> REALLIT
%token <string> STRING
%token <id> ID
%token <string> ASSIGN RBRAC DOT REPEAT FUNCTION COMMA VAL END LBRAC WHILE YBEGIN OUTPUT PROGRAM ELSE SEMIC COLON PARAMSTR IF UNTIL DO THEN VAR FORWARD NOT WRITELN RESERVED
%token <string> AND OR MOD DIV DIF LESSEQ GREATEQ

%union {
	int intlit;
	char* reallit, *string, *id;
	struct node *node;
}

%right ELSE THEN
%right ASSIGN
%left '=' '<' '>' DIF LESSEQ GREATEQ
%left '+' '-' OR
%left '*' '/' DIV MOD AND
%left NOT

%type <node> Prog ProgHeading ProgBlock VarPart VarPartAux VarDeclaration IDList IDListAux FuncPart FuncDeclaration FuncHeading FuncHeadingAux FuncIdent FormalParamList FormalParamListAux FormalParams FormalParamsAux FuncBlock StatPart CompStat StatList SemicStatAux Stat WritelnPList CommaExpStrAux Expr ParamList CommaExprAux

%%

Prog:					ProgHeading SEMIC ProgBlock DOT						{$$ = parsing_tree = create_node("Prog", 2, $1, $3);}

ProgHeading: 			PROGRAM ID LBRAC OUTPUT RBRAC						{$$=create_node("ProgHeading", 1, $2);}

ProgBlock: 				VarPart FuncPart StatPart							{$$=create_node("ProgBlock", 3, $1, $2, $3);}

VarPart: 				VAR VarDeclaration SEMIC VarPartAux					{$$=create_node("VarPart", 2, $2, $4);}
		|				%empty												{;}

VarPartAux: 			VarDeclaration SEMIC VarPartAux						{$$=create_node("VarPartAux", 2, $1, $3);}
		|				%empty												{;}

VarDeclaration: 		IDList COLON ID										{$$=create_node("VarDeclaration", 2, $1, $3);}

IDList: 				ID IDListAux										{$$=create_node("IDList", 2, $1, $2);}

IDListAux:				COMMA ID IDListAux									{$$=create_node("IDListAux", 2, $2, $3);}
		|				%empty												{;}

FuncPart:  				FuncDeclaration SEMIC FuncPart						{$$=create_node("FuncPart", 2, $1, $3);}
		|				%empty												{;}

FuncDeclaration: 		FuncHeading SEMIC FORWARD							{$$=create_node("FuncDeclaration", 1, $1);}
		|				FuncIdent SEMIC FuncBlock							{$$=create_node("FuncDeclaration", 2, $1, $3);}
		|				FuncHeading SEMIC FuncBlock							{$$=create_node("FuncDeclaration", 2, $1, $3);}

FuncHeading: 			FUNCTION ID FuncHeadingAux COLON ID					{$$=create_node("FuncHeading", 3, $2, $3, $5);}

FuncHeadingAux:			FormalParamList										{$$=create_node("FuncHeadingAux", 1, $1);}
		|				%empty												{;}

FuncIdent:				FUNCTION ID											{$$=create_node("FuncIdent", 1, $2);}

FormalParamList: 		LBRAC FormalParams FormalParamListAux RBRAC 		{$$=create_node("FormalParamList", 2, $2, $3);}

FormalParamListAux: 	SEMIC FormalParams FormalParamListAux				{$$=create_node("FormalParamListAux", 2, $2, $3);}
		|				%empty												{;}

FormalParams:			FormalParamsAux IDList COLON ID						{$$=create_node("FormalParams", 3, $1, $2, $4);}

FormalParamsAux:		VAR													{$$=create_terminal("FormalParamsAux", $1);}
		|				%empty												{;}

FuncBlock: 				VarPart StatPart									{$$=create_node("FuncBlock", 2, $1, $2);}

StatPart: 				CompStat											{$$=create_node("StatPart", 1, $1);}

CompStat:				YBEGIN StatList END									{$$=create_node("CompStat", 1, $1);}

StatList: 				Stat SemicStatAux									{$$=create_node("StatList", 2, $1, $2);}

SemicStatAux:			SEMIC Stat SemicStatAux								{$$=create_node("SemicStatAux", 2, $2, $3);}
		|				%empty												{;}

Stat: 					CompStat											{$$=create_node("Stat", 1, $1);}
		|				IF Expr THEN Stat ELSE Stat							{$$=create_node("Stat", 3, $2, $4, $6);}
		|				IF Expr THEN Stat									{$$=create_node("Stat", 2, $2, $4);}
		|				WHILE Expr DO Stat									{$$=create_node("Stat", 2, $2, $4);}
		|				REPEAT StatList UNTIL Expr							{$$=create_node("Stat", 2, $2, $4);}
		|				VAL LBRAC PARAMSTR LBRAC Expr RBRAC COMMA ID RBRAC 	{$$=create_node("Stat", 2, $5, $8);}
		|				ID ASSIGN Expr										{$$=create_node("Stat", 2, $1, $3);}
		|				WRITELN WritelnPList								{$$=create_node("Stat", 1, $2);}
		|				WRITELN												{$$=create_terminal("Stat", $1);}
		|				%empty												{;}

WritelnPList:			LBRAC Expr CommaExpStrAux RBRAC						{$$=create_node("WritelnPList", 2, $2, $3);}
		|				LBRAC STRING CommaExpStrAux RBRAC					{$$=create_node("WritelnPList", 2, $2, $3);}

CommaExpStrAux:			COMMA Expr CommaExpStrAux							{$$=create_node("CommaExpStrAux", 2, $2, $3);}
		|				COMMA STRING CommaExpStrAux							{$$=create_node("CommaExpStrAux", 2, $2, $3);}
		|				%empty												{;}

Expr:					Expr AND Expr										{$$=create_node("Expr", 2, $1, $3);}
		|				Expr OR Expr										{$$=create_node("Expr", 2, $1, $3);}
		|		 		Expr '<' Expr										{$$=create_node("Expr", 2, $1, $3);}
		|				Expr '>' Expr										{$$=create_node("Expr", 2, $1, $3);}
		|				Expr '=' Expr										{$$=create_node("Expr", 2, $1, $3);}
		|				Expr DIF Expr										{$$=create_node("Expr", 2, $1, $3);}
		|				Expr LESSEQ Expr									{$$=create_node("Expr", 2, $1, $3);}
		|				Expr GREATEQ Expr									{$$=create_node("Expr", 2, $1, $3);}
		|				Expr '+' Expr										{$$=create_node("Expr", 2, $1, $3);}
		|				Expr '-' Expr										{$$=create_node("Expr", 2, $1, $3);}
		|				Expr '*' Expr										{$$=create_node("Expr", 2, $1, $3);}
		|		 		Expr MOD Expr										{$$=create_node("Expr", 2, $1, $3);}
		|				Expr '/' Expr										{$$=create_node("Expr", 2, $1, $3);}
		|				Expr DIV Expr										{$$=create_node("Expr", 2, $1, $3);}
		|				'+' Expr											{$$=create_node("Expr", 1, $2);}
		|				'-' Expr											{$$=create_node("Expr", 1, $2);}
		|				NOT Expr											{$$=create_node("Expr", 1, $2);}
		|				LBRAC Expr RBRAC									{$$=create_node("Expr", 1, $2);}
		|				INTLIT												{int aux = $1; $$=create_terminal("IntLit", &aux); /*TODO CONFIRMAR SE ISTO FUNCIONA (void ptr)*/}
		|			 	REALLIT												{$$=create_terminal("RealLit", $1);}
		|				ID ParamList										{$$=create_node("Expr", 2, $1, $2);}
		|				ID													{$$=create_terminal("Id", $1);}

ParamList:				LBRAC Expr CommaExprAux RBRAC						{$$=create_node("ParamList", 2, $2, $3);}

CommaExprAux:			COMMA Expr CommaExprAux								{$$=create_node("CommaExprAux", 2, $2, $3);}
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
