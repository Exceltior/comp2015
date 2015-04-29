%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#define OP_PARSING_TREE "-t"
#define OP_SEMANTIC_TABLE "-s"
#define TABLE_SIZE 500

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

/*syntax*/
node* new_node(char* type, void* value) {
	node* n = (node*)malloc(sizeof(node));
	n->type = strdup(type);
	n->value = value;
	n->children = 0;
	n->used = 1;
	return n;
}

node* create_terminal(char* type, int used, void* value) {
	node* n = new_node(type, value);
	n->used = used;
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

	if (!strcmp(type, "StatList")) {
		if (i < 2) {
			parent->used = 0;
		}
	}
	if (!strcmp(type, "Program")) {
		if (i < 4) {
			parent->children = (node**)realloc(parent->children, sizeof(node)*(parent->n_children+1));
			parent->n_children++;
			parent->children[3] = create_terminal("StatList", 1, NULL);
		}
	}

	if (!strcmp(type, "FuncPart")) {
		int k;
		for (k=0;k<parent->n_children;k++) {
			cur = parent->children[k];
			if (!strcmp(cur->type, "FuncPart")) {
				parent->children = (node**)realloc(parent->children, sizeof(node)*(k+cur->n_children));
				parent->n_children = k+cur->n_children;
				for (j=0;j<cur->n_children;j++) {
					parent->children[k] = cur->children[j];
					k++;
				}
			}
		}
	}

	va_end(args);
	return parent;
}

node* create_ifelse(node* a, node* b, node* c) {
	if ((!strcmp(b->type, "Empty")) || ((!strcmp(b->type, "Stat")) && (b->n_children == 0))) {
		b = create_terminal("StatList", 1, NULL);
	}
	if ((!strcmp(c->type, "Empty")) || ((!strcmp(c->type, "Stat")) && (c->n_children == 0))) {
		c = create_terminal("StatList", 1, NULL);
	}
	return create_node("IfElse", 1, 3, a, b, c);
}

node* create_repeat(node *a, node *b) {
	if ((!strcmp(a->type, "Empty")) || ((!strcmp(a->type, "StatList")) && (a->n_children == 0))) {
		a  = create_terminal("StatList", 1, NULL);
	}
	return create_node("Repeat", 1, 2, a, b);
}

node* create_while(node *a, node *b) {
	if ((!strcmp(b->type, "Empty")) || ((b->n_children == 0))) {
		b  = create_terminal("StatList", 1, NULL);
	}
	return create_node("While", 1, 2, a, b);
}

node* create_funcblock(node *a, node* b) {
	if ((!strcmp(b->type, "StatPart")) && (b->n_children == 0)) {
		b = create_terminal("StatList", 1, NULL);
	}
	create_node("FuncBlock", 0, 2, a, b);
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
		printf("%s(%s)\n", n->type, (char*)n->value);
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

/*sematic*/
typedef struct symbol {
	char* name;
	char* type;
	char* flag;
	char* value;
	struct symbol* next;
} symbol;

typedef struct symbol_table {
	symbol* first;
	char* name;
} symbol_table;

symbol_table** table;
int cur_table_index = 2;

symbol_table** new_table(int size) {
	return (symbol_table**)malloc(sizeof(symbol_table)*size);
}

symbol* new_symbol(char* name, char* type, char* flag, char* value) {
	symbol* s = (symbol*)malloc(sizeof(symbol));
	s->name = name;
	s->type = type;
	s->flag = flag;
	s->value = value;
	return s;
}

symbol_table* new_symbol_table(char* name) {
	symbol_table* st = (symbol_table*)malloc(sizeof(symbol_table));
	st->name = strdup(name);
	return st;
}

void insert_symbol(symbol_table* st, char* name, char* type, char* flag, char* value) {
	symbol* first = st->first;
	if (first == NULL) {
		st->first = new_symbol(name, type, flag, value);
		return;
	}
	while(first->next != NULL) {
		first = first->next;
	}
	first->next = new_symbol(name, type, flag, value);
}

void print_symbol_table(symbol_table* st) {
	symbol* first = st->first;
	printf("===== %s Symbol Table =====\n", st->name);
	while(first != NULL) {
		printf("%s\t%s", first->name, first->type);
		if (first->flag != NULL) {
			printf("\t%s", first->flag);
			if (first->value != NULL) {
				printf("\t%s", first->value);
			}
		}
		printf("\n");
		first = first->next;
	}
}

void print_table () {
	int i = 0;
	while(table[i] != NULL) {
		if (i!=0) {
			printf("\n");
		}
		print_symbol_table(table[i]);
		i++;
	}
}

void init_outer_table() {
	table[0] = new_symbol_table("Outer");
	insert_symbol(table[0], "boolean", "_type_", "constant", "_boolean_");
	insert_symbol(table[0], "integer", "_type_", "constant", "_integer_");
	insert_symbol(table[0], "real", "_type_", "constant", "_real_");
	insert_symbol(table[0], "false", "_boolean_", "constant", "_false_");
	insert_symbol(table[0], "true", "_boolean_", "constant", "_true_");
	insert_symbol(table[0], "paramcount", "_function_", NULL, NULL);
	insert_symbol(table[0], "program", "_program_", NULL, NULL);
}

void init_function_symbol_table() {
	table[1] = new_symbol_table("Function");
	insert_symbol(table[1], "paramcount", "_integer_", "return", NULL);
}

void init_table() {
	table = new_table(TABLE_SIZE);
	init_outer_table();
	init_function_symbol_table();
}

void build_table() {
	//TODO percorrer arvore, etc
}

%}

%token <intlit> INTLIT
%token <reallit> REALLIT
%token <string> STRING
%token <id> ID
%token <string> ASSIGN RBRAC DOT REPEAT FUNCTION COMMA VAL END LBRAC WHILE YBEGIN OUTPUT PROGRAM ELSE SEMIC COLON PARAMSTR IF UNTIL DO THEN VAR FORWARD NOT WRITELN RESERVED
%token <string> AND OR MOD DIV DIF LESSEQ GREATEQ

%union {
	char *intlit, *reallit, *string, *id;
	struct node *node;
}

%right THEN
%right ELSE
%right ASSIGN

%type <node> Prog ProgHeading ProgBlock VarPart VarPartAux VarDeclaration IDList IDListAux FuncPart FuncDeclaration FuncHeading FuncHeadingAux FuncIdent FormalParamList FormalParamListAux FormalParams FuncBlock StatPart CompStat StatList SemicStatAux Stat WritelnPList CommaExpStrAux Expr ParamList CommaExprAux IDAux STRINGAux SimpleExpr AddOP Term Factor VarParams Params

%%

Prog:					ProgHeading SEMIC ProgBlock DOT							{$$ = parsing_tree = create_node("Program", 1, 2, $1, $3);}

ProgHeading: 			PROGRAM IDAux LBRAC OUTPUT RBRAC						{$$=create_node("ProgHeading", 0, 1, $2);}

ProgBlock: 				VarPart FuncPart StatPart								{$$=create_node("ProgHeading", 0, 3, $1, $2, $3);}

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
		|				FuncIdent SEMIC FuncBlock								{$$=create_node("FuncDef2", 1, 2, $1, $3);}
		|				FuncHeading SEMIC FuncBlock								{$$=create_node("FuncDef", 1, 2, $1, $3);}


FuncHeading: 			FUNCTION IDAux FuncHeadingAux COLON IDAux				{$$=create_node("FuncHeading", 0, 3, $2, $3, $5);}

FuncHeadingAux:			FormalParamList											{$$=create_node("FuncHeadingAux", 0, 1, $1);}
		|				%empty													{$$=create_terminal("FuncParams", 1, NULL);}

FuncIdent:				FUNCTION IDAux											{$$=create_node("FuncIdent", 0, 1, $2);}

FormalParamList: 		LBRAC FormalParams FormalParamListAux RBRAC 			{$$=create_node("FuncParams", 1, 2, $2, $3);}

FormalParamListAux: 	SEMIC FormalParams FormalParamListAux					{$$=create_node("FormalParamListAux", 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", 0, NULL);}

FormalParams:           VarParams                                               {$$ = create_node("FormalParams", 0, 1, $1);}
        |               Params                                                  {$$ = create_node("FormalParams", 0, 1, $1);}

VarParams:              VAR IDList COLON IDAux                                  {$$=create_node("VarParams", 1, 2, $2, $4);}

Params:                 IDList COLON IDAux                                      {$$=create_node("Params", 1, 2, $1, $3);}

FuncBlock: 				VarPart StatPart										{$$=create_funcblock($1, $2);}

StatPart: 				CompStat												{$$=create_node("StatPart", 0, 1, $1);}

CompStat:				YBEGIN StatList END										{$$=create_node("CompStat", 0, 1, $2);}

StatList: 				Stat SemicStatAux										{$$=create_node("StatList", 1, 2, $1, $2);}

SemicStatAux:			SEMIC Stat SemicStatAux									{$$=create_node("SemicStatAux", 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", 0, NULL);}

Stat: 					CompStat												{$$=create_node("Stat", 0, 1, $1);}
		|				IF Expr THEN Stat ELSE Stat								{$$=create_ifelse($2, $4, $6);}
		|				IF Expr THEN Stat										{$$=create_ifelse($2, $4, create_terminal("StatList", 1, NULL));}
		|				WHILE Expr DO Stat										{$$=create_while($2, $4);}
		|				REPEAT StatList UNTIL Expr								{$$=create_repeat($2, $4);}
		|				VAL LBRAC PARAMSTR LBRAC Expr RBRAC COMMA IDAux RBRAC 	{$$=create_node("ValParam", 1, 2, $5, $8);}
		|				IDAux ASSIGN Expr										{$$=create_node("Assign", 1, 2, $1, $3);}
		|				WRITELN WritelnPList									{$$=create_node("WriteLn", 1, 1, $2);}
		|				WRITELN													{$$=create_terminal("WriteLn", 1, $1);}
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

Term:					Term '/' Factor											{$$=create_node("RealDiv", 1, 2, $1, $3);}
		|				Term '*' Factor											{$$=create_node("Mul", 1, 2, $1, $3);}
		|				Term AND Factor											{$$=create_node("And", 1, 2, $1, $3);}
		|				Term DIV Factor											{$$=create_node("Div", 1, 2, $1, $3);}
		|				Term MOD Factor											{$$=create_node("Mod", 1, 2, $1, $3);}
		|				Factor													{$$=create_node("Factor", 0, 1, $1);}

Factor:					IDAux													{;}
		|				NOT Factor												{$$=create_node("Not", 1, 1, $2);}
		|				LBRAC Expr RBRAC										{$$=create_node("LbracRbrac", 0, 1, $2);}
		|				IDAux ParamList											{$$=create_node("Call", 1, 2, $1, $2);}
		|				INTLIT													{$$=create_terminal("IntLit", 1, $1);}
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
	init_table();
	build_table();
	int i;
	if (!error_flag) {
		for (i=0;i<argc;i++) {
			if (!strcmp(argv[i], OP_PARSING_TREE)) {
				print_node(parsing_tree, 0);
			}
			else if (!strcmp(argv[i], OP_SEMANTIC_TABLE)) {
				print_table();
			}
		}
	}
	return 0;
}
