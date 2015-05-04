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

node* create_node(char* type, int line, int col, int used, int n_children, ...) {
	va_list args;
	va_start(args, n_children);
	register int i = 0, j, c = 0;

	node* parent = new_node(type, NULL);
	parent->used = used;
	parent->line = line;
	parent->col = col;
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
		b = create_terminal("StatList", -1, -1, 1, NULL);
	}
	if ((!strcmp(c->type, "Empty")) || ((!strcmp(c->type, "Stat")) && (c->n_children == 0))) {
		c = create_terminal("StatList", -1, -1, 1, NULL);
	}
	return create_node("IfElse", -1, -1, 1, 3, a, b, c);
}

node* create_repeat(node *a, node *b) {
	if ((!strcmp(a->type, "Empty")) || ((!strcmp(a->type, "StatList")) && (a->n_children == 0))) {
		a  = create_terminal("StatList", yylineno, col-yyleng, 1, NULL);
	}
	return create_node("Repeat", -1, -1, 1, 2, a, b);
}

node* create_while(node *a, node *b) {
	if ((!strcmp(b->type, "Empty")) || ((b->n_children == 0))) {
		b  = create_terminal("StatList", -1, -1, 1, NULL);
	}
	return create_node("While", -1, -1, 1, 2, a, b);
}

node* create_funcblock(node *a, node* b) {
	if ((!strcmp(b->type, "StatPart")) && (b->n_children == 0)) {
		b = create_terminal("StatList", -1, -1, 1, NULL);
	}
	create_node("FuncBlock", -1, -1, 0, 2, a, b);
}

void print_node(node* n, int depth) {
	int i;

	char* ident = (char*)malloc(sizeof(char)*depth*2+1);
	for (i=0;i<depth*2;i++) {
		ident[i] = '.';
	}

	printf("%s %d %d ", ident, n->line, n->col);
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

/*semantic*/
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
	int func_defined;
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
	st->func_defined = 0;
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

char check_defined_on_table(char *name, int table_ind) {
	name = str_to_lower(name);
	symbol* first = table[table_ind]->first;
	while(first != NULL) {
		if (!strcmp(first->name, name)) {
			return 1;
		}
		first = first->next;
	}
	return 0;
}

//Check if id exists and is type on current scope, program scope and outer scope
char check_global_types(char *type) {
    type = str_to_lower(type);
	symbol* first = table[cur_table_index]->first;
	while(first != NULL) {
		if (!strcmp(first->name, type)) {
            if(first->type != NULL && !strcmp(first->type, "type"))
			    return 1;
		}
		first = first->next;
	}
    first = table[2]->first;
	while(first != NULL) {
		if (!strcmp(first->name, type)) {
            if(first->type != NULL && !strcmp(first->type, "type"))
			    return 1;
		}
		first = first->next;
	}
    first = table[0]->first;
	while(first != NULL) {
		if (!strcmp(first->name, type)) {
			if(first->type != NULL && !strcmp(first->type, "type"))
			    return 1;
		}
		first = first->next;
	}
	return 0;
}

//Check if id exists and is type on current scope, program scope and outer scope
char check_global_ids(char *name) {
    name = str_to_lower(name);
	symbol* first = table[cur_table_index]->first;
	while(first != NULL) {
		if (!strcmp(first->name, name)) {
            return 1;
		}
		first = first->next;
	}
    first = table[2]->first;
	while(first != NULL) {
		if (!strcmp(first->name, name)) {
            return 1;
		}
		first = first->next;
	}
    first = table[0]->first;
	while(first != NULL) {
		if (!strcmp(first->name, name)) {
	        return 1;
		}
		first = first->next;
	}
	return 0;
}

/*function identifier expected*/
char check_function_identifier(char* name) {
	name = str_to_lower(name);
	symbol* first = table[2]->first;
	while (first != NULL) {
		if (!strcmp(first->name, name)) {
			if (!strcmp(first->type, "function")) {
				return 1;
			}
			else {
				return 0;
			}
		}
		first = first->next;
	}
	return 0;
}

char check_number_of_arguments(char* name, int n_args) {
	int i = 0, n = 0;
	symbol* first;
	name = str_to_lower(name);
	while(table[i] != NULL) {
		if (!strcmp(table[i]->name, "Function")) {
			first = table[i]->first;
			if (!strcmp(first->name, name)) {
				while (first != NULL) {
					if (first->flag != NULL) {
						if ((!strcmp(first->flag, "param")) || (!strcmp(first->flag, "varparam")))
							n++;
					}
					first = first->next;
				}
				return n;
			}
		}
		i++;
	}
	return 0;
}


char* check_write_value(char* name, char* type) {
	name = str_to_lower(name);
	symbol* first = table[cur_table_index]->first;
	while(first != NULL) {
		if (!strcmp(first->name, "integer"))
			return first->type;
		else if (!strcmp(first->name, "boolean"))
			return first->type;
		else if (!strcmp(first->name, "real"))
			return first->type;
		first = first->next;
	}
	first = table[2]->first;
	while(first != NULL) {
		if (!strcmp(first->name, "integer"))
			return first->type;
		else if (!strcmp(first->name, "boolean"))
			return first->type;
		else if (!strcmp(first->name, "real"))
			return first->type;
		first = first->next;
	}
	first = table[0]->first;
	while(first != NULL) {
		if (!strcmp(first->name, "integer"))
			return first->type;
		else if (!strcmp(first->name, "boolean"))
			return first->type;
		else if (!strcmp(first->name, "real"))
			return first->type;
		first = first->next;
	}
	return 0;
}

char* get_function_return_type(char* name) {
	int i = 0, n = 0;
	symbol* first;
	name = str_to_lower(name);
	while(table[i] != NULL) {
		if (!strcmp(table[i]->name, "Function")) {
			first = table[i]->first;
			if (!strcmp(first->name, name)) {
				return first->type;
			}
		}
		i++;
	}
	return NULL;
}

char check_function(char* name) {
	name = str_to_lower(name);
	symbol* first = table[2]->first;
	while(first != NULL) {
		if(!strcmp(first->name, name)) {
			if(!strcmp(first->type, "function"))
				return 1;
		}
		first = first->next;
	}
	return 0;
}

char check_assignment(node* a, node *b) {
	char* type_a = a->type, *type_b = b->type;
	char* name_a = str_to_lower(a->value), *name_b = str_to_lower(b->value);
	char* var_type_a = strdup("");
	char* var_type_b = strdup("");

	symbol* first;
	if (!strcmp(type_b, "Id")) {
		first = table[cur_table_index]->first;
		while (first != NULL) {
			if (!strcmp(first->name, name_a)) {
				var_type_a = first->type;
			}
			if (!strcmp(first->name, name_b)) {
				var_type_b = first->type;
			}
			first = first->next;
		}
		first = table[2]->first;
		while (first != NULL) {
			if (!strcmp(first->name, name_a)) {
				var_type_a = first->type;
			}
			if (!strcmp(first->name, name_b)) {
				var_type_b = first->type;
			}
			first = first->next;
		}
		if (!strcmp(var_type_a, "function")) {
			var_type_a = get_function_return_type(name_a);
		}
		if (!strcmp(var_type_a, var_type_b)) {
			return 1;
		}
	}
	else if (!strcmp(type_b, "Call")) {
		char* function_name = b->children[0]->value;
		var_type_b = get_function_return_type(function_name);
		first = table[cur_table_index]->first;
		while (first != NULL) {
			if (!strcmp(first->name, name_a)) {
				var_type_a = first->type;
			}
			first = first->next;
		}
		first = table[2]->first;
		while (first != NULL) {
			if (!strcmp(first->name, name_a)) {
				var_type_a = first->type;
			}
			first = first->next;
		}
		if (!strcmp(var_type_a, "function")) {
			var_type_a = get_function_return_type(name_a);
		}
		if (!strcmp(var_type_a, var_type_b)) {
			return 1;
		}
	}
	else {
		return 1;
	}
	printf("Line %d, col %d: Incompatible type in assigment to %s (got _%s_, expected _%s_)\n", b->line, b->col, name_a, var_type_b ,var_type_a);
	return 0;
}

char* get_argument_type(char* name, int index) {
	name = str_to_lower(name);
	symbol* first;
	int j=0, i=0;
	while(table[i] != NULL) {
		if (!strcmp(table[i]->name, "Function")) {
			first = table[i]->first;
			if (!strcmp(first->name, name)) {
				while (first != NULL) {
					if (first->flag != NULL) {
						if ((!strcmp(first->flag, "param")) || (!strcmp(first->flag, "varparam"))) {
							j++;
							if (j == index) {
								return first->type;
							}
						}
					}
					first = first->next;
				}
			}
		}
		i++;
	}
}

char* get_symbol_type(char* name) {
	name = str_to_lower(name);
	symbol* first = table[cur_table_index]->first;
	while(first != NULL) {
		if (!strcmp(first->name, name)) {
			return first->type;
		}
		first = first->next;
	}
	first = table[2]->first;
	while(first != NULL) {
		if (!strcmp(first->name, name)) {
			if (!strcmp(first->type, "function")) {
				return get_function_return_type(first->name);
			}
			return first->type;
		}
		first = first->next;
	}
	return NULL;
}

char is_expression(char* type) {
	if (!strcmp(type, "Add"))
		return 1;
	if (!strcmp(type, "Sub"))
		return 1;
	if (!strcmp(type, "Or"))
		return 1;
	if (!strcmp(type, "Minus"))
		return 1;
	if (!strcmp(type, "Plus"))
		return 1;
	if (!strcmp(type, "RealDiv"))
		return 1;
	if (!strcmp(type, "Mul"))
		return 1;
	if (!strcmp(type, "And"))
		return 1;
	if (!strcmp(type, "Div"))
		return 1;
	if (!strcmp(type, "Mod"))
		return 1;
	if (!strcmp(type, "Not"))
		return 1;
	if (!strcmp(type, "Leq"))
		return 1;
	if (!strcmp(type, "Geq"))
		return 1;
	if (!strcmp(type, "Gt"))
		return 1;
	if (!strcmp(type, "Lt"))
		return 1;
	if (!strcmp(type, "Neq"))
		return 1;
	if (!strcmp(type, "Eq"))
		return 1;
	return 0;
}

char check_unary_operator(node* n) {
	node* c = n->children[0];
	char* type;
	if (c->value == NULL)
		return 1;
	if (!strcmp(c->type, "Id")) {
		type = get_symbol_type(c->value);
	}
	else if (!strcmp(c->type, "Call")) {
		return 1;
	}
	else if (!strcmp(c->value, "IntLit")) {
		type = strdup("integer");
	}
	else if (!strcmp(c->value, "RealLit")) {
		type = strdup("real");
	}
	else if (!strcmp(c->value, "String")) {
		type = strdup("string");
	}
	if ((!strcmp(c->value, "false")) || (!strcmp(c->value, "true"))) {
		type = strdup("boolean");
	}
	if (type == NULL)
		return 1;
	if ((!strcmp(n->type, "Minus")) || (!strcmp(n->type, "Plus"))) {
		if ((!strcmp(type, "integer")) || (!strcmp(type, "real"))) {
			return 1;
		}
		else {
			printf("Line %d, col %d: Operator ", n->line, n->col);
			if (!strcmp(n->type, "Minus")) {
				printf("-");
			}
			else {
				printf("+");
			}
			printf(" cannot be applied to type _%s_\n", type);
			return 0;
		}
	}
	else if (!strcmp(n->type, "Not")) {
		if (!strcmp(type, "boolean")) {
			return 1;
		}
		else {
			printf("Line %d, col %d: Operator not cannot be applied to type _%s_\n", n->line, n->col, type);
			return 0;
		}
	}
}

char check_normal_operator(node* n) {
	return 1;
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
		symbol_type = n->children[2]->value;
		symbol_line = n->children[2]->line;
		//Check function return type
		symbol_col = n->children[2]->col;
		if (!check_global_ids(symbol_type)) {
            printf("Line %d, col %d: Symbol %s not defined\n", symbol_line, symbol_col, symbol_type);
            exit(0);
        }
        else if (!check_global_types(symbol_type)) {
            printf("Line %d, col %d: Type identifier expected\n", symbol_line, symbol_col);
            exit(0);
        }
		//Check function name
		if (!check_defined_on_table(n->children[0]->value, 2)) {
			insert_symbol(table[2], n->children[0]->value, "function", NULL, NULL);
		}
		else {
			printf("Line %d, col %d: Symbol %s already defined\n", (int)n->children[0]->line, (int)n->children[0]->col, (char*)n->children[0]->value);
			exit(0);
		}
		while(table[cur_table_index] != NULL) {
			cur_table_index++;
		}
		table[cur_table_index] = new_symbol_table("Function");
		if(!strcmp(n->type, "FuncDef"))
			table[cur_table_index]->func_defined = 1;
		insert_symbol(table[cur_table_index], n->children[0]->value, symbol_type, "return", NULL);
	}
	else if (!strcmp(n->type, "FuncDef2")) {
		int index = -1;
		i = 0;
		symbol_line = n->children[0]->line;
		symbol_col = n->children[0]->col;
		if (!check_function_identifier(n->children[0]->value)) {
			printf("Line %d, col %d: Function identifier expected\n", symbol_line, symbol_col);
			exit(0);
		}
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
			printf("Line %d, col %d: Function identifier expected\n", symbol_line, symbol_col);
			exit(0);
		}
		if(table[cur_table_index]->func_defined) {
			printf("Line %d, col %d: Symbol %s already defined\n", (int)n->children[0]->line, (int)n->children[0]->col, (char*)n->children[0]->value);
			exit(0);
		}
		table[cur_table_index]->func_defined = 1;
	}
    //New error checkings for VarDecl
	else if (!strcmp(n->type, "VarDecl")) {
		symbol_type = n->children[n->n_children-1]->value;
		symbol_line = n->children[n->n_children-1]->line;
		symbol_col = n->children[n->n_children-1]->col;
        if (!check_global_ids(symbol_type)) {
            printf("Line %d, col %d: Symbol %s not defined\n", symbol_line, symbol_col, symbol_type);
            exit(0);
        }
        else if (!check_global_types(symbol_type)) {
            printf("Line %d, col %d: Type identifier expected\n", symbol_line, symbol_col);
            exit(0);
        }
        for (i=0;i<n->n_children-1;i++) {
			if (!check_already_defined(n->children[i]->value)) {
				insert_symbol(table[cur_table_index], n->children[i]->value, symbol_type, NULL, NULL);
			}
			else {
				printf("Line %d, col %d: Symbol %s already defined\n", (int)n->children[i]->line, (int)n->children[i]->col, (char*)n->children[i]->value);
				exit(0);
			}
		}
	}
	else if (!strcmp(n->type, "Params")) {
		symbol_type = n->children[n->n_children-1]->value;
		symbol_line = n->children[n->n_children-1]->line;
		symbol_col = n->children[n->n_children-1]->col;
		//Check types
		if (!check_global_ids(symbol_type)) {
            printf("Line %d, col %d: Symbol %s not defined\n", symbol_line, symbol_col, symbol_type);
            exit(0);
        }
        else if (!check_global_types(symbol_type)) {
            printf("Line %d, col %d: Type identifier expected\n", symbol_line, symbol_col);
            exit(0);
        }
		for (i=0;i<n->n_children-1;i++) {
			if(!check_already_defined(n->children[i]->value)) {
				insert_symbol(table[cur_table_index], n->children[i]->value, symbol_type, "param", NULL);
			}
			else {
				printf("Line %d, col %d: Symbol %s already defined\n", (int)n->children[i]->line, (int)n->children[i]->col, (char*)n->children[i]->value);
				exit(0);
			}
		}
	}
	else if (!strcmp(n->type, "VarParams")) {
		symbol_type = n->children[n->n_children-1]->value;
		symbol_line = n->children[n->n_children-1]->line;
		symbol_col = n->children[n->n_children-1]->col;
		//Check types
		if (!check_global_ids(symbol_type)) {
            printf("Line %d, col %d: Symbol %s not defined\n", symbol_line, symbol_col, symbol_type);
            exit(0);
        }
        else if (!check_global_types(symbol_type)) {
            printf("Line %d, col %d: Type identifier expected\n", symbol_line, symbol_col);
            exit(0);
        }
		for (i=0;i<n->n_children-1;i++) {
			if(!check_already_defined(n->children[i]->value)) {
				insert_symbol(table[cur_table_index], n->children[i]->value, symbol_type, "varparam", NULL);
			}
			else {
				printf("Line %d, col %d: Symbol %s already defined\n", (int)n->children[i]->line, (int)n->children[i]->col, (char*)n->children[i]->value);
				exit(0);
			}
		}
	}
	else if (!strcmp(n->type, "Call")) {
		if (n->n_children > 0) {
			char* name = n->children[0]->value;
			if (name != NULL) {
				if(!check_defined_on_table(name, 2)) {
					printf("Line %d, col %d: Symbol %s not defined\n", (int)n->children[0]->line, (int)n->children[0]->col, (char*)n->children[0]->value);
					exit(0);
				}
				else if(!check_function(name)) {
					printf("Line %d, col %d: Function identifier expected\n", n->children[0]->line, n->children[0]->col);
					exit(0);
				}
				int expected = check_number_of_arguments(name, n->n_children-1);
				symbol_line = n->children[0]->line;
				symbol_col = n->children[0]->col;
				if (expected != n->n_children-1) {
					printf("Line %d, col %d: Wrong number of arguments in call to function %s (got %d, expected %d)\n", symbol_line, symbol_col, name, n->n_children-1, expected);
					exit(0);
				}
				/*if (n->n_children > 0) {
					for (i=1;i<n->n_children;i++) {
						char* arg_type = get_argument_type(name, i);
						char* cur_arg_type;
						if (!strcmp(n->children[i]->type, "Call")) {
							cur_arg_type = get_symbol_type(n->children[i]->children[0]->value);
						}
						else if (!strcmp(n->children[i]->type, "Id")) {
							cur_arg_type = get_symbol_type(n->children[i]->value);
						}
						//TODO EXPRESSIONS
						else break;
						symbol_line = n->children[i]->line;
						symbol_col = n->children[i]->col;
						if ((arg_type != NULL) && (cur_arg_type != NULL)) {
							if (strcmp(arg_type, cur_arg_type)) {
								printf("Line %d, col %d: Incompatible type for argument %d in call to function %s (got _%s_, expected _%s_)\n", symbol_line, symbol_col, i+1, name, cur_arg_type, arg_type);
								exit(0);
							}
						}
					}
				}*/
			}
		}
	}
	else if (!strcmp(n->type, "WriteLn")) {
		int i;
		char* name;
		for (i=0;i<n->n_children;i++) {
			name = n->children[i]->value;
			symbol_line = n->children[i]->line;
			symbol_col = n->children[i]->col;
			if (!strcmp(n->children[i]->type, "Call")) {
				char* return_type = get_function_return_type(n->children[i]->children[0]->value);
				if (return_type != NULL) {
					if (strcmp(return_type, "integer") && (strcmp(return_type, "real")) && (strcmp(return_type, "boolean"))) {
						printf("Line %d, col %d: Cannot write values of type _%s_\n", symbol_line, symbol_col, return_type);
						exit(0);
					}
				}
			}
			else if (!strcmp(n->children[i]->type, "Id")) {
				/*if(!check_defined_on_table(name, 2)) {
					printf("Line %d, col %d: Symbol %s not defined\n", (int)n->children[0]->line, (int)n->children[0]->col, (char*)n->children[0]->value);
					exit(0);
				}*/
				/*if(!check_defined_on_table(name, cur_table_index)) {
					printf("Line %d, col %d: Symbol %s not defined\n", (int)n->children[0]->line, (int)n->children[0]->col, (char*)n->children[0]->value);
					exit(0);
				}*/
				char* write_type = check_write_value(name, n->children[i]->type);
				if (write_type != NULL) {
					if (strcmp(write_type, "integer") && (strcmp(write_type, "real")) && (strcmp(write_type, "boolean"))) {
						printf("Line %d, col %d: Cannot write values of type _%s_\n", symbol_line, symbol_col, write_type);
						exit(0);
					}
				}
			}
		}
	}
	else if (!strcmp(n->type, "Assign")) {
		if (check_defined_on_table(n->children[0]->value, 0) || check_function_identifier(n->children[0]->value)) {
			if (strcmp(n->children[0]->value, table[cur_table_index]->first->name)) {
				printf("Line %d, col %d: Variable identifier expected\n", n->children[0]->line, n->children[0]->col);
				exit(0);
			}
		}
		if (!check_global_ids(n->children[0]->value)) {
			printf("Line %d, col %d: Symbol %s not defined\n", n->children[0]->line, n->children[0]->col, n->children[0]->value);
			exit(0);
		}
		/*if (!check_assignment(n->children[0], n->children[1])) {
			exit(0);
		}*/
	}
	else if (is_expression(n->type)) {
		if (n->n_children == 1) {
			if (!check_unary_operator(n)) {
				exit(0);
			}
		}
		else if (n->n_children == 2) {
			if (!check_normal_operator(n)) {
				exit(0);
			}
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

Prog:					ProgHeading SEMIC ProgBlock DOT							{$$ = parsing_tree = create_node("Program", -1, -1, 1, 2, $1, $3);}

ProgHeading: 			PROGRAM IDAux LBRAC OUTPUT RBRAC						{$$=create_node("ProgHeading", -1, -1, 0, 1, $2);}

ProgBlock: 				VarPart FuncPart StatPart								{$$=create_node("ProgHeading", -1, -1, 0, 3, $1, $2, $3);}

VarPart: 				VAR VarDeclaration SEMIC VarPartAux						{$$=create_node("VarPart", -1, -1, 1, 2, $2, $4);}
		|				%empty													{$$=create_terminal("VarPart", -1, -1, 1, NULL);}

VarPartAux: 			VarDeclaration SEMIC VarPartAux							{$$=create_node("VarPartAux", -1, -1, 0, 2, $1, $3);}
		|				%empty													{$$=create_terminal("Empty", -1, -1, 0, NULL);}

VarDeclaration: 		IDList COLON IDAux										{$$=create_node("VarDecl", -1, -1, 1, 2, $1, $3);}

IDList: 				IDAux IDListAux											{$$=create_node("IDList", -1, -1, 0, 2, $1, $2);}

IDListAux:				COMMA IDAux IDListAux									{$$=create_node("IDListAux", -1, -1, 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", -1, -1, 0, NULL);}

FuncPart:  				FuncDeclaration SEMIC FuncPart							{$$=create_node("FuncPart", -1, -1, 1, 2, $1, $3);}
		|				%empty													{$$=create_terminal("FuncPart", -1, -1, 1, NULL);}

FuncDeclaration: 		FuncHeading SEMIC FORWARD								{$$=create_node("FuncDecl", -1, -1, 1, 1, $1);}
		|				FuncIdent SEMIC FuncBlock								{$$=create_node("FuncDef2", -1, -1, 1, 2, $1, $3);}
		|				FuncHeading SEMIC FuncBlock								{$$=create_node("FuncDef", -1, -1, 1, 2, $1, $3);}


FuncHeading: 			FUNCTION IDAux FuncHeadingAux COLON IDAux				{$$=create_node("FuncHeading", -1, -1, 0, 3, $2, $3, $5);}

FuncHeadingAux:			FormalParamList											{$$=create_node("FuncHeadingAux", -1, -1, 0, 1, $1);}
		|				%empty													{$$=create_terminal("FuncParams", -1, -1, 1, NULL);}

FuncIdent:				FUNCTION IDAux											{$$=create_node("FuncIdent", -1, -1, 0, 1, $2);}

FormalParamList: 		LBRAC FormalParams FormalParamListAux RBRAC 			{$$=create_node("FuncParams", -1, -1, 1, 2, $2, $3);}

FormalParamListAux: 	SEMIC FormalParams FormalParamListAux					{$$=create_node("FormalParamListAux", -1, -1, 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", -1, -1, 0, NULL);}

FormalParams:           VarParams                                               {$$ = create_node("FormalParams", -1, -1, 0, 1, $1);}
        |               Params                                                  {$$ = create_node("FormalParams", -1, -1, 0, 1, $1);}

VarParams:              VAR IDList COLON IDAux                                  {$$=create_node("VarParams", -1, -1, 1, 2, $2, $4);}

Params:                 IDList COLON IDAux                                      {$$=create_node("Params", -1, -1, 1, 2, $1, $3);}

FuncBlock: 				VarPart StatPart										{$$=create_funcblock($1, $2);}

StatPart: 				CompStat												{$$=create_node("StatPart", -1, -1, 0, 1, $1);}

CompStat:				YBEGIN StatList END										{$$=create_node("CompStat", -1, -1, 0, 1, $2);}

StatList: 				Stat SemicStatAux										{$$=create_node("StatList", -1, -1, 1, 2, $1, $2);}

SemicStatAux:			SEMIC Stat SemicStatAux									{$$=create_node("SemicStatAux", -1, -1, 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", -1, -1, 0, NULL);}

Stat: 					CompStat												{$$=create_node("Stat", -1, -1, 0, 1, $1);}
		|				IF Expr THEN Stat ELSE Stat								{$$=create_ifelse($2, $4, $6);}
		|				IF Expr THEN Stat										{$$=create_ifelse($2, $4, create_terminal("StatList", yylineno, col-yyleng, 1, NULL));}
		|				WHILE Expr DO Stat										{$$=create_while($2, $4);}
		|				REPEAT StatList UNTIL Expr								{$$=create_repeat($2, $4);}
		|				VAL LBRAC PARAMSTR LBRAC Expr RBRAC COMMA IDAux RBRAC 	{$$=create_node("ValParam", -1, -1, 1, 2, $5, $8);}
		|				IDAux ASSIGN Expr										{$$=create_node("Assign", -1, -1, 1, 2, $1, $3);}
		|				WRITELN WritelnPList									{$$=create_node("WriteLn", -1, -1, 1, 1, $2);}
		|				WRITELN													{$$=create_terminal("WriteLn", -1, -1, 1, $1);}
		|				%empty													{$$=create_terminal("Empty", -1, -1, 0, NULL);}

WritelnPList:			LBRAC Expr CommaExpStrAux RBRAC							{$$=create_node("WritelnPList", -1, -1, 0, 2, $2, $3);}
		|				LBRAC STRINGAux CommaExpStrAux RBRAC					{$$=create_node("WritelnPList", -1, -1, 0, 2, $2, $3);}

CommaExpStrAux:			COMMA Expr CommaExpStrAux								{$$=create_node("CommaExpStrAux", -1, -1, 0, 2, $2, $3);}
		|				COMMA STRINGAux CommaExpStrAux							{$$=create_node("CommaExpStrAux", -1, -1, 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", -1, -1, 0, NULL);}

Expr:					SimpleExpr '=' SimpleExpr								{$$=create_node("Eq", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				SimpleExpr DIF SimpleExpr								{$$=create_node("Neq", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				SimpleExpr '<' SimpleExpr								{$$=create_node("Lt", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				SimpleExpr '>' SimpleExpr								{$$=create_node("Gt", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				SimpleExpr LESSEQ SimpleExpr							{$$=create_node("Leq", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				SimpleExpr GREATEQ SimpleExpr							{$$=create_node("Geq", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				SimpleExpr												{$$=create_node("SimpleExpr", -1, -1, 0, 1, $1);}

SimpleExpr:				Term													{$$=create_node("Term", -1, -1, 0, 1, $1);}
		|				AddOP													{$$=create_node("AddOP", -1, -1, 0, 1, $1);}

AddOP:					SimpleExpr '+' Term										{$$=create_node("Add", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				SimpleExpr '-' Term										{$$=create_node("Sub", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				SimpleExpr OR Term										{$$=create_node("Or", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				'-' Term												{$$=create_node("Minus", @1.first_line, @1.first_column, 1, 1, $2);}
		|				'+' Term												{$$=create_node("Plus", @1.first_line, @1.first_column, 1, 1, $2);}

Term:					Term '/' Factor											{$$=create_node("RealDiv", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				Term '*' Factor											{$$=create_node("Mul", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				Term AND Factor											{$$=create_node("And", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				Term DIV Factor											{$$=create_node("Div", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				Term MOD Factor											{$$=create_node("Mod", @2.first_line, @2.first_column, 1, 2, $1, $3);}
		|				Factor													{$$=create_node("Factor", -1, -1, 0, 1, $1);}

Factor:					IDAux													{;}
		|				NOT Factor												{$$=create_node("Not", @1.first_line, @1.first_column, 1, 1, $2);}
		|				LBRAC Expr RBRAC										{$$=create_node("LbracRbrac", -1, -1, 0, 1, $2);}
		|				IDAux ParamList											{$$=create_node("Call", @1.first_line, @1.first_column, 1, 2, $1, $2);}
		|				INTLIT													{$$=create_terminal("IntLit", @1.first_line, @1.first_column, 1, $1);}
		|				REALLIT													{$$=create_terminal("RealLit", @1.first_line, @1.first_column, 1, $1);}

ParamList:				LBRAC Expr CommaExprAux RBRAC							{$$=create_node("ParamList", -1, -1, 0, 2, $2, $3);}

CommaExprAux:			COMMA Expr CommaExprAux									{$$=create_node("CommaExprAux", -1, -1, 0, 2, $2, $3);}
		|				%empty													{$$=create_terminal("Empty", -1, -1, 0, NULL);}

IDAux:					ID														{$$=create_terminal("Id", @1.first_line, @1.first_column, 1, $1);}

STRINGAux:				STRING													{$$=create_terminal("String", @1.first_line, @1.first_column, 1, $1);}

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
