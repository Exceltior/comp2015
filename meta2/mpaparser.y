%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#define OP_PARSING_TREE "-t"

typedef struct node {
	char* type;
	void* value;
	int n_children;
	char used;
	struct node** children;
} node;

node* parsing_tree;

int error_flag;
extern int yyleng, yylineno, col;
extern char* yytext;

int stat_counter = 0;

node* new_node(char* type, void* value) {
	node* n = (node*)malloc(sizeof(node));
	n->type = strdup(type);
	n->value = value;
	n->children = 0;
	n->used = 1;
	return n;
}

node* create_node(char* type, int used, int n_children, ...) {
	va_list args;
	va_start(args, n_children);
	register int i = 0, j, c = 0;

	node* parent = new_node(type, NULL);
	parent->used = used;
	parent->children = (node**)malloc(sizeof(node)*n_children);
	parent->n_children = n_children;

	node* cur;
	for (c=0;c<n_children;c++) {
		cur = va_arg(args, node*);
		if (cur->used) {
			parent->children[i] = cur;
			i++;
		}
		else {
			parent->children = (node**)realloc(parent->children, sizeof(node)*(parent->n_children-1+cur->n_children));
			parent->n_children = (parent->n_children-1)+cur->n_children;
			for (j=0;j<cur->n_children;j++) {
				parent->children[i] = cur->children[j];
				i++;
			}
		}
	}
	va_end(args);
	return parent;
}

node* create_terminal(char* type, int used, void* value) {
	node* n = new_node(type, value);
	n->used = used;
	return n;
}

void print_node(node* n, int depth) {
	int i;

	char* ident = (char*)malloc(sizeof(char)*depth*2+1);
	for (i=0;i<depth*2;i++) {
		ident[i] = '.';
	}

	printf("%s", ident);
	if (!strcmp(n->type, "Id")) {
		printf("%s(%s)\n", n->type, (char*)n->value);
	}
	else if (!strcmp(n->type, "IntLit")) {
		printf("%s(%d)\n", n->type, *((int*)n->value));
	}
	else if (!strcmp(n->type, "RealLit")) {
		printf("%s(%s)\n", n->type, (char*)n->value);
	}
	else if (!strcmp(n->type, "String")) {
		printf("%s(%s)\n", n->type, (char*)n->value);
	}
	else {
		printf("%s\n", n->type);
	}

	for (i=0;i<n->n_children;i++) {
		print_node(n->children[i], depth+1);
	}
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

%type <node> Prog ProgHeading ProgBlock VarPart VarPartAux VarDeclaration IDList IDListAux FuncPart FuncDeclaration FuncHeading FuncHeadingAux FuncIdent FormalParamList FormalParamListAux FormalParams FormalParamsAux FuncBlock StatPart CompStat StatList SemicStatAux Stat WritelnPList CommaExpStrAux Expr ParamList CommaExprAux IDAux STRINGAux SimpleExpr AddOP Term Factor

%%

Prog:					ProgHeading SEMIC ProgBlock DOT							{$$ = parsing_tree = create_node("Program", 1, 2, $1, $3);}

ProgHeading: 			PROGRAM IDAux LBRAC OUTPUT RBRAC						{$$=create_node("ProgHeading", 0, 1, $2);}

ProgBlock: 				VarPart FuncPart StatPart								{$$=create_node("ProgBlock", 0, 3, $1, $2, $3);}

VarPart: 				VAR VarDeclaration SEMIC VarPartAux						{$$=create_node("VarPart", 1, 2, $2, $4);}
		|				%empty													{$$=create_terminal("VarPart", 1, NULL);}

VarPartAux: 			VarDeclaration SEMIC VarPartAux							{$$=create_node("VarPartAux", 0, 2, $1, $3);}
		|				%empty													{$$=create_terminal("Empty", 0, NULL);}

VarDeclaration: 		IDList COLON IDAux										{$$=create_node("VarDecl", 1, 2, $1, $3);}

IDList: 				IDAux IDListAux											{$$=create_node("IDList", 0, 2, $1, $2);}

IDListAux:				COMMA IDAux IDListAux									{$$=create_node("IDListAux", 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", 0, NULL);}

FuncPart:  				FuncDeclaration SEMIC FuncPart							{$$=create_node("FuncPart", 1, 2, $1, $3);}
		|				%empty													{$$=create_terminal("FuncPart", 1, NULL);}

FuncDeclaration: 		FuncHeading SEMIC FORWARD								{$$=create_node("FuncDecl", 1, 1, $1);}
		|				FuncIdent SEMIC FuncBlock								{$$=create_node("FuncDecl", 1, 2, $1, $3);}
		|				FuncHeading SEMIC FuncBlock								{$$=create_node("FuncDecl", 1, 2, $1, $3);}

FuncHeading: 			FUNCTION IDAux FuncHeadingAux COLON IDAux				{$$=create_node("FuncHeading", 0, 3, $2, $3, $5);}

FuncHeadingAux:			FormalParamList											{$$=create_node("FuncHeadingAux", 0, 1, $1);}
		|				%empty													{$$=create_terminal("Empty", 0, NULL);}

FuncIdent:				FUNCTION IDAux											{$$=create_node("FuncIdent", 0, 1, $2);}

FormalParamList: 		LBRAC FormalParams FormalParamListAux RBRAC 			{$$=create_node("FuncParams", 1, 2, $2, $3);}

FormalParamListAux: 	SEMIC FormalParams FormalParamListAux					{$$=create_node("FormalParamListAux", 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", 0, NULL);}

FormalParams:			FormalParamsAux IDList COLON IDAux						{$$=create_node("VarParams", 1, 3, $1, $2, $4);}

FormalParamsAux:		VAR														{$$=create_terminal("FormalParamsAux", 0, $1);}
		|				%empty													{$$=create_terminal("Empty", 0, NULL);}

FuncBlock: 				VarPart StatPart										{$$=create_node("FuncBlock", 0, 2, $1, $2);}

StatPart: 				CompStat												{$$=create_node("StatPart", 0, 1, $1);}

CompStat:				YBEGIN StatList END										{$$=create_node("CompStat", 0, 1, $2);}

StatList: 				Stat SemicStatAux										{if (stat_counter != 1) $$=create_node("StatList", 1, 2, $1, $2); stat_counter = 0;}

SemicStatAux:			SEMIC Stat SemicStatAux									{$$=create_node("SemicStatAux", 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", 0, NULL);}

Stat: 					CompStat												{stat_counter++; $$=create_node("Stat", 0, 1, $1);}
		|				IF Expr THEN Stat ELSE Stat								{stat_counter++; $$=create_node("IfElse", 1, 3, $2, $4, $6);}
		|				IF Expr THEN Stat										{stat_counter++; $$=create_node("IfElse", 1, 2, $2, $4);}
		|				WHILE Expr DO Stat										{stat_counter++; $$=create_node("While", 1, 2, $2, $4);}
		|				REPEAT StatList UNTIL Expr								{stat_counter++; $$=create_node("Repeat", 1, 2, $2, $4);}
		|				VAL LBRAC PARAMSTR LBRAC Expr RBRAC COMMA IDAux RBRAC 	{stat_counter++; $$=create_node("ValParam", 1, 2, $5, $8);}
		|				IDAux ASSIGN Expr										{stat_counter++; $$=create_node("Assign", 1, 2, $1, $3);}
		|				WRITELN WritelnPList									{stat_counter++; $$=create_node("WriteLn", 1, 1, $2);}
		|				WRITELN													{stat_counter++; $$=create_terminal("WriteLn", 1, $1);}
		|				%empty													{$$=create_terminal("Empty", 0, NULL);}

WritelnPList:			LBRAC Expr CommaExpStrAux RBRAC							{$$=create_node("WritelnPList", 0, 2, $2, $3);}
		|				LBRAC STRINGAux CommaExpStrAux RBRAC					{$$=create_node("WritelnPList", 0, 2, $2, $3);}

CommaExpStrAux:			COMMA Expr CommaExpStrAux								{$$=create_node("CommaExpStrAux", 0, 2, $2, $3);}
		|				COMMA STRINGAux CommaExpStrAux							{$$=create_node("CommaExpStrAux", 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", 0, NULL);}

Expr:					SimpleExpr '=' SimpleExpr								{$$=create_node("Eq", 1, 2, $1, $3);}
		|				SimpleExpr DIF SimpleExpr								{$$=create_node("Neq", 1, 2, $1, $3);}
		|				SimpleExpr '<' SimpleExpr								{$$=create_node("Lt", 1, 2, $1, $3);}
		|				SimpleExpr '>' SimpleExpr								{$$=create_node("Gt", 1, 2, $1, $3);}
		|				SimpleExpr LESSEQ SimpleExpr							{$$=create_node("Leq", 1, 2, $1, $3);}
		|				SimpleExpr GREATEQ SimpleExpr							{$$=create_node("Geq", 1, 2, $1, $3);}
		|				SimpleExpr												{$$=create_node("SimpleExpr", 0, 1, $1);}

SimpleExpr:				Term													{$$=create_node("Term", 0, 1, $1);}
		|				AddOP													{$$=create_node("AddOP", 0, 1, $1);}

AddOP:					SimpleExpr '+' Term										{$$=create_node("Add", 1, 2, $1, $3);}
		|				SimpleExpr '-' Term										{$$=create_node("Sub", 1, 2, $1, $3);}
		|				SimpleExpr OR Term										{$$=create_node("Or", 1, 2, $1, $3);}
		|				'-' Term												{$$=create_node("Minus", 1, 1, $2);}
		|				'+' Term												{$$=create_node("Plus", 1, 1, $2);}

Term:					Term '/' Factor											{$$=create_node("Div", 1, 2, $1, $3);}
		|				Term '*' Factor											{$$=create_node("Mul", 1, 2, $1, $3);}
		|				Term AND Factor											{$$=create_node("And", 1, 2, $1, $3);}
		|				Term DIV Factor											{$$=create_node("RealDiv", 1, 2, $1, $3);}
		|				Term MOD Factor											{$$=create_node("Mod", 1, 2, $1, $3);}
		|				Factor													{$$=create_node("Factor", 0, 1, $1);}

Factor:					IDAux													{;}
		|				NOT Factor												{$$=create_node("Not", 1, 1, $2);}
		|				LBRAC Expr RBRAC										{$$=create_node("LbracRbrac", 1, 1, $2);}
		|				IDAux ParamList											{$$=create_node("Call", 1, 2, $1, $2);}
		|				INTLIT													{int *aux = (int*)malloc(sizeof(int)); *aux = $1; $$=create_terminal("IntLit", 1, aux);}
		|				REALLIT													{$$=create_terminal("RealLit", 1, $1);}

ParamList:				LBRAC Expr CommaExprAux RBRAC							{$$=create_node("ParamList", 0, 2, $2, $3);}

CommaExprAux:			COMMA Expr CommaExprAux									{$$=create_node("CommaExprAux", 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", 0, NULL);}

IDAux:					ID														{$$=create_terminal("Id", 1, $1);}

STRINGAux:				STRING													{$$=create_terminal("String", 1, $1);}

%%
int yyerror() {
	error_flag = 1;
	printf("Line %d, col %d: syntax error: %s\n", yylineno, col - yyleng, yytext);
}

int main(int argc, char** argv) {
	yyparse();
	if (argc > 1) {
		if (!strcmp(argv[1], OP_PARSING_TREE) && !error_flag) {
			print_node(parsing_tree, 0);
		}
	}
	return 0;
}
