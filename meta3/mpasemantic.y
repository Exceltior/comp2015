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
	int line, col;
} node;

node* parsing_tree;

int error_flag;
extern int yyleng, yylineno, col;
extern char* yytext;

/*parsing tree*/
node* new_node(char* type, void* value) {
	node* n = (node*)malloc(sizeof(node));
	n->type = strdup(type);
	n->value = value;
	n->children = 0;
	n->used = 1;
	return n;
}

node* create_terminal(char* type, int line, int col, int used, void* value) {
	node* n = new_node(type, value);
	n->line = line;
	n->col = col;
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
			parent->children[3] = create_terminal("StatList", yylineno, col-yyleng, 1, NULL);
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
		b = create_terminal("StatList", yylineno, col-yyleng, 1, NULL);
	}
	if ((!strcmp(c->type, "Empty")) || ((!strcmp(c->type, "Stat")) && (c->n_children == 0))) {
		c = create_terminal("StatList", yylineno, col-yyleng, 1, NULL);
	}
	return create_node("IfElse", 1, 3, a, b, c);
}

node* create_repeat(node *a, node *b) {
	if ((!strcmp(a->type, "Empty")) || ((!strcmp(a->type, "StatList")) && (a->n_children == 0))) {
		a  = create_terminal("StatList", yylineno, col-yyleng, 1, NULL);
	}
	return create_node("Repeat", 1, 2, a, b);
}

node* create_while(node *a, node *b) {
	if ((!strcmp(b->type, "Empty")) || ((b->n_children == 0))) {
		b  = create_terminal("StatList", yylineno, col-yyleng, 1, NULL);
	}
	return create_node("While", 1, 2, a, b);
}

node* create_funcblock(node *a, node* b) {
	if ((!strcmp(b->type, "StatPart")) && (b->n_children == 0)) {
		b = create_terminal("StatList", yylineno, col-yyleng, 1, NULL);
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
int cur_table_index = 1;

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

char* str_to_lower(char* str) {
	if (str == NULL)
		return NULL;
	int i;
	char* s = (char*)malloc(sizeof(char)*strlen(str));
	for (i=0;i<strlen(str);i++) {
		s[i] = tolower(str[i]);
	}
	return s;
}

/*Errors*/
char check_symbol_type(char* type) {
	symbol* first = table[0]->first;
	while(first != NULL) {
		if (!strcmp(first->name, type)) {
			return 1;
		}
		first = first->next;
	}
	return 0;
}

char check_already_defined(char* name) {
	name = str_to_lower(name);
	symbol* first = table[cur_table_index]->first;
	while(first != NULL) {
		if (!strcmp(first->name, name)) {
			return 1;
		}
		first = first->next;
	}
	return 0;
}

void insert_symbol(symbol_table* st, char* name, char* type, char* flag, char* value) {
	name = str_to_lower(name);
	type = str_to_lower(type);
	flag = str_to_lower(flag);
	value = str_to_lower(value);
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
		printf("%s\t_%s_", first->name, first->type);
		if (first->flag != NULL) {
			printf("\t%s", first->flag);
			if (first->value != NULL) {
				printf("\t_%s_", first->value);
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
	insert_symbol(table[0], "boolean", "type", "constant", "boolean");
	insert_symbol(table[0], "integer", "type", "constant", "integer");
	insert_symbol(table[0], "real", "type", "constant", "real");
	insert_symbol(table[0], "false", "boolean", "constant", "false");
	insert_symbol(table[0], "true", "boolean", "constant", "true");
	insert_symbol(table[0], "paramcount", "function", NULL, NULL);
	insert_symbol(table[0], "program", "program", NULL, NULL);
}

void init_function_symbol_table() {
	table[1] = new_symbol_table("Function");
	insert_symbol(table[1], "paramcount", "integer", "return", NULL);
}

void init_table() {
	table = new_table(TABLE_SIZE);
	init_outer_table();
	init_function_symbol_table();
}

char build_table(node* n) {
	int i, symbol_line, symbol_col;
	char* symbol_type;
	if (!strcmp(n->type, "Program")) {
		cur_table_index++;
		table[cur_table_index] = new_symbol_table("Program");
	}
	else if ((!strcmp(n->type, "FuncDef")) || (!strcmp(n->type, "FuncDecl"))) {
		insert_symbol(table[2], n->children[0]->value, "function", NULL, NULL);
		while(table[cur_table_index] != NULL) {
			cur_table_index++;
		}
		table[cur_table_index] = new_symbol_table("Function");
		symbol_type = n->children[2]->value;
		symbol_line = n->children[2]->line;
		symbol_col = n->children[2]->col;
		if (!check_symbol_type(n->children[2]->value)) {
			printf("Line %d, col %d: Type identifier expected\n", symbol_line, symbol_col);
			exit(0);
		}
		insert_symbol(table[cur_table_index], n->children[0]->value, symbol_type, "return", NULL);
	}
	else if (!strcmp(n->type, "FuncDef2")) {
		int index = -1;
		i = 0;
		while(table[i] != NULL) {
			if (!strcmp(table[i]->first->name, str_to_lower(n->children[0]->value))) {
				index = i;
				break;
			}
			i++;
		}
		if (index != -1) {
			cur_table_index = index;
		}
		else {
			/*TODO ERRORS*/
		}
	}
	else if (!strcmp(n->type, "VarDecl")) {
		symbol_type = n->children[n->n_children-1]->value;
		symbol_line = n->children[n->n_children-1]->line;
		symbol_col = n->children[n->n_children-1]->col;
		if (!check_symbol_type(symbol_type)) {
			printf("Line %d, col %d: Type identifier expected\n", symbol_line, symbol_col);
			exit(0);
		}
		for (i=0;i<n->n_children-1;i++) {
			if (!check_already_defined(n->children[i]->value)) {
				insert_symbol(table[cur_table_index], n->children[i]->value, symbol_type, NULL, NULL);
			}
			else {
				printf("Line %d, col %d: Symbol %s already defined\n", symbol_line, symbol_col, n->children[i]->value);
				exit(0);
			}

		}
	}
	else if (!strcmp(n->type, "Params")) {
		symbol_type = n->children[n->n_children-1]->value;
		symbol_line = n->children[n->n_children-1]->line;
		symbol_col = n->children[n->n_children-1]->col;
		if (!check_symbol_type(symbol_type)) {
			printf("Line %d, col %d: Type identifier expected\n", symbol_line, symbol_col);
			exit(0);
		}
		for (i=0;i<n->n_children-1;i++) {
			insert_symbol(table[cur_table_index], n->children[i]->value, symbol_type, "param", NULL);
		}
	}
	else if (!strcmp(n->type, "VarParams")) {
		symbol_type = n->children[n->n_children-1]->value;
		symbol_line = n->children[n->n_children-1]->line;
		symbol_col = n->children[n->n_children-1]->col;
		if (!check_symbol_type(symbol_type)) {
			printf("Line %d, col %d: Type identifier expected\n", symbol_line, symbol_col);
			return;
		}
		for (i=0;i<n->n_children-1;i++) {
			insert_symbol(table[cur_table_index], n->children[i]->value, symbol_type, "varparam", NULL);
		}
	}
	for (i=0;i<n->n_children;i++) {
		build_table(n->children[i]);
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
		|				%empty													{$$=create_terminal("VarPart", yylineno, col-yyleng, 1, NULL);}

VarPartAux: 			VarDeclaration SEMIC VarPartAux							{$$=create_node("VarPartAux", 0, 2, $1, $3);}
		|				%empty													{$$=create_terminal("Empty", 0, 0, 0, NULL);}

VarDeclaration: 		IDList COLON IDAux										{$$=create_node("VarDecl", 1, 2, $1, $3);}

IDList: 				IDAux IDListAux											{$$=create_node("IDList", 0, 2, $1, $2);}

IDListAux:				COMMA IDAux IDListAux									{$$=create_node("IDListAux", 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", 0, 0, 0, NULL);}

FuncPart:  				FuncDeclaration SEMIC FuncPart							{$$=create_node("FuncPart", 1, 2, $1, $3);}
		|				%empty													{$$=create_terminal("FuncPart", yylineno, col-yyleng, 1, NULL);}

FuncDeclaration: 		FuncHeading SEMIC FORWARD								{$$=create_node("FuncDecl", 1, 1, $1);}
		|				FuncIdent SEMIC FuncBlock								{$$=create_node("FuncDef2", 1, 2, $1, $3);}
		|				FuncHeading SEMIC FuncBlock								{$$=create_node("FuncDef", 1, 2, $1, $3);}


FuncHeading: 			FUNCTION IDAux FuncHeadingAux COLON IDAux				{$$=create_node("FuncHeading", 0, 3, $2, $3, $5);}

FuncHeadingAux:			FormalParamList											{$$=create_node("FuncHeadingAux", 0, 1, $1);}
		|				%empty													{$$=create_terminal("FuncParams", yylineno, col-yyleng, 1, NULL);}

FuncIdent:				FUNCTION IDAux											{$$=create_node("FuncIdent", 0, 1, $2);}

FormalParamList: 		LBRAC FormalParams FormalParamListAux RBRAC 			{$$=create_node("FuncParams", 1, 2, $2, $3);}

FormalParamListAux: 	SEMIC FormalParams FormalParamListAux					{$$=create_node("FormalParamListAux", 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", 0, 0, 0, NULL);}

FormalParams:           VarParams                                               {$$ = create_node("FormalParams", 0, 1, $1);}
        |               Params                                                  {$$ = create_node("FormalParams", 0, 1, $1);}

VarParams:              VAR IDList COLON IDAux                                  {$$=create_node("VarParams", 1, 2, $2, $4);}

Params:                 IDList COLON IDAux                                      {$$=create_node("Params", 1, 2, $1, $3);}

FuncBlock: 				VarPart StatPart										{$$=create_funcblock($1, $2);}

StatPart: 				CompStat												{$$=create_node("StatPart", 0, 1, $1);}

CompStat:				YBEGIN StatList END										{$$=create_node("CompStat", 0, 1, $2);}

StatList: 				Stat SemicStatAux										{$$=create_node("StatList", 1, 2, $1, $2);}

SemicStatAux:			SEMIC Stat SemicStatAux									{$$=create_node("SemicStatAux", 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", 0, 0, 0, NULL);}

Stat: 					CompStat												{$$=create_node("Stat", 0, 1, $1);}
		|				IF Expr THEN Stat ELSE Stat								{$$=create_ifelse($2, $4, $6);}
		|				IF Expr THEN Stat										{$$=create_ifelse($2, $4, create_terminal("StatList", yylineno, col-yyleng, 1, NULL));}
		|				WHILE Expr DO Stat										{$$=create_while($2, $4);}
		|				REPEAT StatList UNTIL Expr								{$$=create_repeat($2, $4);}
		|				VAL LBRAC PARAMSTR LBRAC Expr RBRAC COMMA IDAux RBRAC 	{$$=create_node("ValParam", 1, 2, $5, $8);}
		|				IDAux ASSIGN Expr										{$$=create_node("Assign", 1, 2, $1, $3);}
		|				WRITELN WritelnPList									{$$=create_node("WriteLn", 1, 1, $2);}
		|				WRITELN													{$$=create_terminal("WriteLn", yylineno, col-yyleng, 1, $1);}
		|				%empty													{$$=create_terminal("Empty", 0, 0, 0, NULL);}

WritelnPList:			LBRAC Expr CommaExpStrAux RBRAC							{$$=create_node("WritelnPList", 0, 2, $2, $3);}
		|				LBRAC STRINGAux CommaExpStrAux RBRAC					{$$=create_node("WritelnPList", 0, 2, $2, $3);}

CommaExpStrAux:			COMMA Expr CommaExpStrAux								{$$=create_node("CommaExpStrAux", 0, 2, $2, $3);}
		|				COMMA STRINGAux CommaExpStrAux							{$$=create_node("CommaExpStrAux", 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", 0, 0, 0, NULL);}

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
		|				INTLIT													{$$=create_terminal("IntLit", yylineno, col-yyleng, 1, $1);}
		|				REALLIT													{$$=create_terminal("RealLit", yylineno, col-yyleng, 1, $1);}

ParamList:				LBRAC Expr CommaExprAux RBRAC							{$$=create_node("ParamList", 0, 2, $2, $3);}

CommaExprAux:			COMMA Expr CommaExprAux									{$$=create_node("CommaExprAux", 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", yylineno, col-yyleng, 0, NULL);}

IDAux:					ID														{$$=create_terminal("Id", yylineno, col-yyleng, 1, $1);}

STRINGAux:				STRING													{$$=create_terminal("String", yylineno, col-yyleng, 1, $1);}

%%
int yyerror() {
	error_flag = 1;
	printf("Line %d, col %d: syntax error: %s\n", yylineno, col - yyleng, yytext);
}

int main(int argc, char** argv) {
	yyparse();
	if (!error_flag) {
		init_table();
		build_table(parsing_tree);
	}
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
