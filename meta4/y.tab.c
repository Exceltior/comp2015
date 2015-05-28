/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "mpacompile.y" /* yacc.c:339  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#define OP_PARSING_TREE "-t"
#define OP_SEMANTIC_TABLE "-s"
#define TABLE_SIZE 2000
#define MAX_STRINGS 1000
#define MAXLEN 100

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

char* get_type(node* c) {
	char* type;
	if (!strcmp(c->type, "Id")) {
		type = get_symbol_type(c->value);
	}
	else if (!strcmp(c->type, "Call")) {
		type = get_function_return_type(c->children[0]->value);
	}
	else if (!strcmp(c->type, "IntLit")) {
		type = strdup("integer");
	}
	else if (!strcmp(c->type, "RealLit")) {
		type = strdup("real");
	}
	else if (!strcmp(c->type, "String")) {
		type = strdup("string");
	}
	if ((!strcmp(c->value, "false")) || (!strcmp(c->value, "true"))) {
		type = strdup("boolean");
	}
	return type;
}

char check_unary_operator(node* n) {
	node* c = n->children[0];
	char* type;
	if (c->value == NULL)
		return 1;
	type = get_type(c);
	if (type == NULL)
		return 1;

	if ((!strcmp(n->type, "Minus")) || (!strcmp(n->type, "Plus"))) {
		if ((!strcmp(type, "integer")) || (!strcmp(type, "real"))) {
			return 1;
		}
		else {
			if (!strcmp(n->type, "Minus")) {
				printf("Line %d, col %d: Operator - cannot be applied to type _%s_\n", n->line, n->col, type);
			}
			else {
				printf("Line %d, col %d: Operator + cannot be applied to type _%s_\n", n->line, n->col, type);
			}
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
	node* a = n->children[0];
	node* b = n->children[1];
	if (a->value == NULL)
		return 1;
	if (b->value == NULL)
		return 1;
	char* a_type = get_type(a);
	char* b_type = get_type(b);
	if (a_type == NULL)
		return 1;
	if (b_type == NULL)
		return 1;
	if (!strcmp(n->type, "Mod")) {
		if ((!strcmp(a_type, "real")) || (!strcmp(b_type, "real")) || (!strcmp(a_type, "boolean")) || (!strcmp(b_type, "boolean"))) {
			printf("Line %d, col %d: Operator Mod cannot be applied to types _%s_, _%s_\n", n->line, n->col, a_type, b_type);
			return 0;
		}
	}
	else if ((!strcmp(a_type, "boolean")) || (!strcmp(b_type, "boolean"))) {
		if ((!strcmp(n->type, "And")) || (!strcmp(n->type, "Or")) || (!strcmp(n->type, "Eq")) || (!strcmp(n->type, "Neq"))) {
			return 1;
		}
		else {
			if (!strcmp(n->type, "Add")) {
				printf("Line %d, col %d: Operator + cannot be applied to types _%s_, _%s_\n", n->line, n->col, a_type, b_type);
			}
			else if (!strcmp(n->type, "Sub")) {
				printf("Line %d, col %d: Operator - cannot be applied to types _%s_, _%s_\n", n->line, n->col, a_type, b_type);
			}
			else if (!strcmp(n->type, "Mul")) {
				printf("Line %d, col %d: Operator * cannot be applied to types _%s_, _%s_\n", n->line, n->col, a_type, b_type);
			}
			else if (!strcmp(n->type, "Div")) {
				printf("Line %d, col %d: Operator div cannot be applied to types _%s_, _%s_\n", n->line, n->col, a_type, b_type);
			}
			else if (!strcmp(n->type, "RealDiv")) {
				printf("Line %d, col %d: Operator / cannot be applied to types _%s_, _%s_\n", n->line, n->col, a_type, b_type);
			}
			else if (!strcmp(n->type, "Mod")) {
				printf("Line %d, col %d: Operator mod cannot be applied to types _%s_, _%s_\n", n->line, n->col, a_type, b_type);
			}
			else if (!strcmp(n->type, "Leq")) {
				printf("Line %d, col %d: Operator <= cannot be applied to types _%s_, _%s_\n", n->line, n->col, a_type, b_type);
			}
			else if (!strcmp(n->type, "Geq")) {
				printf("Line %d, col %d: Operator >= cannot be applied to types _%s_, _%s_\n", n->line, n->col, a_type, b_type);
			}
			else if (!strcmp(n->type, "Gt")) {
				printf("Line %d, col %d: Operator > cannot be applied to types _%s_, _%s_\n", n->line, n->col, a_type, b_type);
			}
			else if (!strcmp(n->type, "Lt")) {
				printf("Line %d, col %d: Operator < cannot be applied to types _%s_, _%s_\n", n->line, n->col, a_type, b_type);
			}
			return 0;
		}
	}
	else if ((!strcmp(a_type, "string")) || (!strcmp(b_type, "string")) || (!strcmp(a_type, "integer")) || (!strcmp(b_type, "integer")) || (!strcmp(a_type, "real")) || (!strcmp(b_type, "real"))) {
		if (!strcmp(n->type, "And")) {
			printf("Line %d, col %d: Operator and cannot be applied to types _%s_, _%s_\n", n->line, n->col, a_type, b_type);
			return 0;
		}
		else if (!strcmp(n->type, "Or")) {
			printf("Line %d, col %d: Operator or cannot be applied to types _%s_, _%s_\n", n->line, n->col, a_type, b_type);
			return 0;
		}
	}
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
				if (!check_global_ids(name)) {
					printf("Line %d, col %d: Symbol %s not defined\n", (int)n->children[i]->line, (int)n->children[i]->col, (char*)n->children[i]->value);
					exit(0);
				}
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

/*CODE*/
int current_string = 0;
int current_scope_string;
char global_strings[MAX_STRINGS][MAXLEN];

void generateGlobalVars() {
	//TODO
}

void generateFunctions() {
	//TODO
}


void generateMainInstructions(node* n) {
	int i;
	if (!strcmp(n->type, "WriteLn")) {
		if (!strcmp(n->children[0]->type, "String")) {
			char* tmp_str = n->children[0]->value;
			printf("\t%%tmp_str%d = getelementptr [%d x i8]* @.global_str%d, i64 0, i64 0\n", current_scope_string, strlen(tmp_str)-1, current_string);
			printf("\tcall i32 @puts(i8* %%tmp_str%d)\n", current_scope_string);
			strcpy(global_strings[current_string], tmp_str);
			current_string++;
			current_scope_string++;
		}
	}
	for (i=0;i<n->n_children;i++) {
		generateMainInstructions(n->children[i]);
	}
}

void generateGlobalStrings() {
	int i = 0;
	char* cur_str = global_strings[0];
	while (cur_str[0] != 0) {
		cur_str++;
		cur_str[strlen(cur_str)-1] = 0;
		printf("@.global_str%d = private unnamed_addr constant [%d x i8] c\"%s\\00\"\n", i, strlen(cur_str)+1, cur_str);
		i++;
		cur_str = global_strings[i];
	}
	printf("\n");
}

void generateMain() {
	printf("define i32 @main() {\n");
	//varPart
	//TODO

	//ĩnstructions
	current_scope_string = 0;
	generateMainInstructions(parsing_tree->children[3]);
	printf("\tret i32 0\n}\n");
	printf("\n");
}

void generateDeclarations() {
	printf("declare i32 @puts(i8* nocapture) nounwind\n");
	printf("\n");
}

void generateCode() {
	generateDeclarations();
	generateGlobalVars();
	generateFunctions();
	generateMain();
	generateGlobalStrings();
}


#line 1135 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INTLIT = 258,
    REALLIT = 259,
    STRING = 260,
    ID = 261,
    ASSIGN = 262,
    RBRAC = 263,
    DOT = 264,
    REPEAT = 265,
    FUNCTION = 266,
    COMMA = 267,
    VAL = 268,
    END = 269,
    LBRAC = 270,
    WHILE = 271,
    YBEGIN = 272,
    OUTPUT = 273,
    PROGRAM = 274,
    ELSE = 275,
    SEMIC = 276,
    COLON = 277,
    PARAMSTR = 278,
    IF = 279,
    UNTIL = 280,
    DO = 281,
    THEN = 282,
    VAR = 283,
    FORWARD = 284,
    NOT = 285,
    WRITELN = 286,
    RESERVED = 287,
    AND = 288,
    OR = 289,
    MOD = 290,
    DIV = 291,
    DIF = 292,
    LESSEQ = 293,
    GREATEQ = 294
  };
#endif
/* Tokens.  */
#define INTLIT 258
#define REALLIT 259
#define STRING 260
#define ID 261
#define ASSIGN 262
#define RBRAC 263
#define DOT 264
#define REPEAT 265
#define FUNCTION 266
#define COMMA 267
#define VAL 268
#define END 269
#define LBRAC 270
#define WHILE 271
#define YBEGIN 272
#define OUTPUT 273
#define PROGRAM 274
#define ELSE 275
#define SEMIC 276
#define COLON 277
#define PARAMSTR 278
#define IF 279
#define UNTIL 280
#define DO 281
#define THEN 282
#define VAR 283
#define FORWARD 284
#define NOT 285
#define WRITELN 286
#define RESERVED 287
#define AND 288
#define OR 289
#define MOD 290
#define DIV 291
#define DIF 292
#define LESSEQ 293
#define GREATEQ 294

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 1077 "mpacompile.y" /* yacc.c:355  */

	char *intlit, *reallit, *string, *id;
	struct node *node;

#line 1258 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 1289 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   165

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  80
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  168

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    46,    43,     2,    44,     2,    45,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      41,    40,    42,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1090,  1090,  1092,  1094,  1096,  1097,  1099,  1100,  1102,
    1104,  1106,  1107,  1109,  1110,  1112,  1113,  1114,  1117,  1119,
    1120,  1122,  1124,  1126,  1127,  1129,  1130,  1132,  1134,  1136,
    1138,  1140,  1142,  1144,  1145,  1147,  1148,  1149,  1150,  1151,
    1152,  1153,  1154,  1155,  1156,  1158,  1159,  1161,  1162,  1163,
    1165,  1166,  1167,  1168,  1169,  1170,  1171,  1173,  1174,  1176,
    1177,  1178,  1179,  1180,  1182,  1183,  1184,  1185,  1186,  1187,
    1189,  1190,  1191,  1192,  1193,  1194,  1196,  1198,  1199,  1201,
    1203
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTLIT", "REALLIT", "STRING", "ID",
  "ASSIGN", "RBRAC", "DOT", "REPEAT", "FUNCTION", "COMMA", "VAL", "END",
  "LBRAC", "WHILE", "YBEGIN", "OUTPUT", "PROGRAM", "ELSE", "SEMIC",
  "COLON", "PARAMSTR", "IF", "UNTIL", "DO", "THEN", "VAR", "FORWARD",
  "NOT", "WRITELN", "RESERVED", "AND", "OR", "MOD", "DIV", "DIF", "LESSEQ",
  "GREATEQ", "'='", "'<'", "'>'", "'+'", "'-'", "'/'", "'*'", "$accept",
  "Prog", "ProgHeading", "ProgBlock", "VarPart", "VarPartAux",
  "VarDeclaration", "IDList", "IDListAux", "FuncPart", "FuncDeclaration",
  "FuncHeading", "FuncHeadingAux", "FuncIdent", "FormalParamList",
  "FormalParamListAux", "FormalParams", "VarParams", "Params", "FuncBlock",
  "StatPart", "CompStat", "StatList", "SemicStatAux", "Stat",
  "WritelnPList", "CommaExpStrAux", "Expr", "SimpleExpr", "AddOP", "Term",
  "Factor", "ParamList", "CommaExprAux", "IDAux", "STRINGAux", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
      61,    60,    62,    43,    45,    47,    42
};
# endif

#define YYPACT_NINF -108

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-108)))

#define YYTABLE_NINF -22

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -10,    23,    32,    12,  -108,    20,  -108,    11,    37,    23,
      49,    48,    52,    44,    46,    59,  -108,    23,    55,    53,
      54,    58,  -108,    23,    23,    23,  -108,    -2,   111,  -108,
    -108,    48,   -17,    11,  -108,    60,  -108,    59,     0,    62,
    -108,   111,    61,    63,    63,    67,  -108,    71,    68,    81,
    -108,  -108,    55,  -108,  -108,    23,  -108,    23,    72,    74,
    -108,  -108,    23,    73,    78,  -108,  -108,    63,   108,   108,
     108,    77,   106,  -108,   -15,  -108,    82,    83,    47,  -108,
    -108,   111,  -108,    63,  -108,  -108,    86,    23,     0,    96,
    -108,    63,    90,   107,  -108,   -15,   -15,   111,   108,    63,
      63,    63,    63,    63,    63,   108,   108,   108,   108,   108,
     108,   108,    63,  -108,   111,  -108,   104,   104,    68,  -108,
      23,  -108,    74,  -108,  -108,    63,  -108,  -108,   -15,   -29,
     -29,   -29,   -29,   -29,   -29,   -15,   -15,  -108,  -108,  -108,
    -108,  -108,   110,    98,    47,   112,   117,  -108,  -108,  -108,
     118,    63,   128,   111,   104,   104,  -108,  -108,   125,   110,
    -108,  -108,  -108,  -108,    23,  -108,   131,  -108
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,    79,     0,     1,     6,     0,     0,
       0,    14,     0,     0,     0,    12,     2,     0,     0,     0,
       0,     0,     3,     8,     0,     0,    10,    20,    44,     4,
      30,    14,     6,     6,     5,     0,     9,    12,     0,     0,
      19,    44,     0,     0,     0,    43,    35,     0,    34,     0,
      13,    15,     0,    17,    16,     8,    11,     0,     0,    24,
      25,    26,     0,     0,     0,    74,    75,     0,     0,     0,
       0,     0,    56,    58,    57,    69,    70,     0,     0,    42,
      31,    44,    32,     0,    29,     7,     0,     0,     0,     0,
      18,     0,     0,     0,    71,    63,    62,    44,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    73,    44,    80,    49,    49,    34,    41,
       0,    28,    24,    22,    39,     0,    72,    38,    61,    51,
      54,    55,    50,    52,    53,    59,    60,    66,    68,    67,
      64,    65,    78,    37,     0,     0,     0,    33,    27,    23,
       0,     0,     0,    44,    49,    49,    45,    46,     0,    78,
      76,    36,    47,    48,     0,    77,     0,    40
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -108,  -108,  -108,  -108,   134,    99,   142,   -31,   116,   124,
    -108,  -108,  -108,  -108,  -108,    34,    69,  -108,  -108,   126,
     109,   -14,   119,    40,   -80,  -108,  -107,   -42,    30,  -108,
      -6,   -65,  -108,     3,    -1,    21
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,    10,    52,    34,    35,    14,    26,    18,
      19,    20,    39,    21,    40,    89,    59,    60,    61,    53,
      29,    46,    47,    82,    48,    79,   145,    71,    72,    73,
      74,    75,   113,   152,    76,   117
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       5,   118,    77,    94,    30,    98,     4,    58,    15,     1,
     146,     9,    51,    38,   105,   106,    27,   127,   107,   -21,
     108,   109,    15,    36,    37,    93,    86,    49,    57,     4,
     110,   111,     6,     7,   143,     8,   116,    15,    30,     9,
      49,   119,   137,   138,   139,   140,   141,   162,   163,   124,
      65,    66,   115,     4,    15,    12,    15,    58,    16,    17,
      22,    90,    67,    95,    96,    23,    65,    66,    24,     4,
     142,    25,    28,   161,    31,    32,    64,    68,    67,    33,
      49,    55,    78,   150,    62,    80,   121,    15,    83,    81,
      69,    70,   128,    68,    87,    88,    49,   112,    91,   135,
     136,    92,   154,    97,   123,   125,    69,    70,   120,   159,
     114,    65,    66,    49,     4,   126,   144,     4,   153,   148,
     156,    41,   151,    67,    42,   157,   158,    43,    28,   129,
     130,   131,   132,   133,   134,    44,   160,   164,    68,   167,
      98,    11,    45,    99,   100,   101,   102,   103,   104,   105,
     106,    13,    49,    56,    85,    50,   149,   122,   147,    54,
      63,    84,   165,   166,     0,   155
};

static const yytype_int16 yycheck[] =
{
       1,    81,    44,    68,    18,    34,     6,    38,     9,    19,
     117,    28,    29,    15,    43,    44,    17,    97,    33,    21,
      35,    36,    23,    24,    25,    67,    57,    28,    28,     6,
      45,    46,     0,    21,   114,    15,    78,    38,    52,    28,
      41,    83,   107,   108,   109,   110,   111,   154,   155,    91,
       3,     4,     5,     6,    55,    18,    57,    88,     9,    11,
       8,    62,    15,    69,    70,    21,     3,     4,    22,     6,
     112,    12,    17,   153,    21,    21,    15,    30,    15,    21,
      81,    21,    15,   125,    22,    14,    87,    88,     7,    21,
      43,    44,    98,    30,    22,    21,    97,    15,    25,   105,
     106,    23,   144,    26,     8,    15,    43,    44,    22,   151,
      27,     3,     4,   114,     6,     8,    12,     6,    20,   120,
       8,    10,    12,    15,    13,     8,     8,    16,    17,    99,
     100,   101,   102,   103,   104,    24,     8,    12,    30,     8,
      34,     7,    31,    37,    38,    39,    40,    41,    42,    43,
      44,     9,   153,    37,    55,    31,   122,    88,   118,    33,
      41,    52,   159,   164,    -1,   144
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    19,    48,    49,     6,    81,     0,    21,    15,    28,
      50,    51,    18,    53,    54,    81,     9,    11,    56,    57,
      58,    60,     8,    21,    22,    12,    55,    81,    17,    67,
      68,    21,    21,    21,    52,    53,    81,    81,    15,    59,
      61,    10,    13,    16,    24,    31,    68,    69,    71,    81,
      56,    29,    51,    66,    66,    21,    55,    28,    54,    63,
      64,    65,    22,    69,    15,     3,     4,    15,    30,    43,
      44,    74,    75,    76,    77,    78,    81,    74,    15,    72,
      14,    21,    70,     7,    67,    52,    54,    22,    21,    62,
      81,    25,    23,    74,    78,    77,    77,    26,    34,    37,
      38,    39,    40,    41,    42,    43,    44,    33,    35,    36,
      45,    46,    15,    79,    27,     5,    74,    82,    71,    74,
      22,    81,    63,     8,    74,    15,     8,    71,    77,    75,
      75,    75,    75,    75,    75,    77,    77,    78,    78,    78,
      78,    78,    74,    71,    12,    73,    73,    70,    81,    62,
      74,    12,    80,    20,    74,    82,     8,     8,     8,    74,
       8,    71,    73,    73,    12,    80,    81,     8
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    49,    50,    51,    51,    52,    52,    53,
      54,    55,    55,    56,    56,    57,    57,    57,    58,    59,
      59,    60,    61,    62,    62,    63,    63,    64,    65,    66,
      67,    68,    69,    70,    70,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    72,    72,    73,    73,    73,
      74,    74,    74,    74,    74,    74,    74,    75,    75,    76,
      76,    76,    76,    76,    77,    77,    77,    77,    77,    77,
      78,    78,    78,    78,    78,    78,    79,    80,    80,    81,
      82
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     5,     3,     4,     0,     3,     0,     3,
       2,     3,     0,     3,     0,     3,     3,     3,     5,     1,
       0,     2,     4,     3,     0,     1,     1,     4,     3,     2,
       1,     3,     2,     3,     0,     1,     6,     4,     4,     4,
       9,     3,     2,     1,     0,     4,     4,     3,     3,     0,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     3,
       3,     3,     2,     2,     3,     3,     3,     3,     3,     1,
       1,     2,     3,     2,     1,     1,     4,     3,     0,     1,
       1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 1090 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node) = parsing_tree = create_node("Program", -1, -1, 1, 2, (yyvsp[-3].node), (yyvsp[-1].node));}
#line 2578 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 1092 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("ProgHeading", -1, -1, 0, 1, (yyvsp[-3].node));}
#line 2584 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 1094 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("ProgHeading", -1, -1, 0, 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));}
#line 2590 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 1096 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("VarPart", -1, -1, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2596 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 1097 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("VarPart", -1, -1, 1, NULL);}
#line 2602 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 1099 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("VarPartAux", -1, -1, 0, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2608 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 1100 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", -1, -1, 0, NULL);}
#line 2614 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 1102 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("VarDecl", -1, -1, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2620 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 1104 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("IDList", -1, -1, 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2626 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 1106 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("IDListAux", -1, -1, 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2632 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 1107 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", -1, -1, 0, NULL);}
#line 2638 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 1109 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncPart", -1, -1, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2644 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 1110 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("FuncPart", -1, -1, 1, NULL);}
#line 2650 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 1112 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncDecl", -1, -1, 1, 1, (yyvsp[-2].node));}
#line 2656 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 1113 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncDef2", -1, -1, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2662 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 1114 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncDef", -1, -1, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2668 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 1117 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncHeading", -1, -1, 0, 3, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[0].node));}
#line 2674 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 1119 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncHeadingAux", -1, -1, 0, 1, (yyvsp[0].node));}
#line 2680 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 1120 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("FuncParams", -1, -1, 1, NULL);}
#line 2686 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 1122 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncIdent", -1, -1, 0, 1, (yyvsp[0].node));}
#line 2692 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 1124 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncParams", -1, -1, 1, 2, (yyvsp[-2].node), (yyvsp[-1].node));}
#line 2698 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 1126 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FormalParamListAux", -1, -1, 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2704 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 1127 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", -1, -1, 0, NULL);}
#line 2710 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 1129 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node) = create_node("FormalParams", -1, -1, 0, 1, (yyvsp[0].node));}
#line 2716 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 1130 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node) = create_node("FormalParams", -1, -1, 0, 1, (yyvsp[0].node));}
#line 2722 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 1132 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("VarParams", -1, -1, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2728 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 1134 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Params", -1, -1, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2734 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 1136 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_funcblock((yyvsp[-1].node), (yyvsp[0].node));}
#line 2740 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 1138 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("StatPart", -1, -1, 0, 1, (yyvsp[0].node));}
#line 2746 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 1140 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("CompStat", -1, -1, 0, 1, (yyvsp[-1].node));}
#line 2752 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 1142 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("StatList", -1, -1, 1, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2758 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 1144 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("SemicStatAux", -1, -1, 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2764 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 1145 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", -1, -1, 0, NULL);}
#line 2770 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 1147 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Stat", -1, -1, 0, 1, (yyvsp[0].node));}
#line 2776 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 1148 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_ifelse((yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));}
#line 2782 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 1149 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_ifelse((yyvsp[-2].node), (yyvsp[0].node), create_terminal("StatList", yylineno, col-yyleng, 1, NULL));}
#line 2788 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 1150 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_while((yyvsp[-2].node), (yyvsp[0].node));}
#line 2794 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 1151 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_repeat((yyvsp[-2].node), (yyvsp[0].node));}
#line 2800 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 1152 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("ValParam", -1, -1, 1, 2, (yyvsp[-4].node), (yyvsp[-1].node));}
#line 2806 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 1153 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Assign", -1, -1, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2812 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 1154 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("WriteLn", -1, -1, 1, 1, (yyvsp[0].node));}
#line 2818 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 1155 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("WriteLn", -1, -1, 1, (yyvsp[0].string));}
#line 2824 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 1156 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", -1, -1, 0, NULL);}
#line 2830 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 1158 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("WritelnPList", -1, -1, 0, 2, (yyvsp[-2].node), (yyvsp[-1].node));}
#line 2836 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 1159 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("WritelnPList", -1, -1, 0, 2, (yyvsp[-2].node), (yyvsp[-1].node));}
#line 2842 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 1161 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("CommaExpStrAux", -1, -1, 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2848 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 1162 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("CommaExpStrAux", -1, -1, 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2854 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 1163 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", -1, -1, 0, NULL);}
#line 2860 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 1165 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Eq", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2866 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 1166 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Neq", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2872 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 1167 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Lt", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2878 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 1168 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Gt", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2884 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 1169 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Leq", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2890 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 1170 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Geq", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2896 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 1171 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("SimpleExpr", -1, -1, 0, 1, (yyvsp[0].node));}
#line 2902 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 1173 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Term", -1, -1, 0, 1, (yyvsp[0].node));}
#line 2908 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 1174 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("AddOP", -1, -1, 0, 1, (yyvsp[0].node));}
#line 2914 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 1176 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Add", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2920 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 1177 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Sub", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2926 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1178 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Or", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2932 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1179 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Minus", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 1, (yyvsp[0].node));}
#line 2938 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1180 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Plus", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 1, (yyvsp[0].node));}
#line 2944 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1182 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("RealDiv", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2950 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1183 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Mul", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2956 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1184 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("And", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2962 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1185 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Div", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2968 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1186 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Mod", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2974 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1187 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Factor", -1, -1, 0, 1, (yyvsp[0].node));}
#line 2980 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1189 "mpacompile.y" /* yacc.c:1646  */
    {;}
#line 2986 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 1190 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Not", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 1, (yyvsp[0].node));}
#line 2992 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 1191 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("LbracRbrac", -1, -1, 0, 1, (yyvsp[-1].node));}
#line 2998 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1192 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Call", (yylsp[-1]).first_line, (yylsp[-1]).first_column, 1, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 3004 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1193 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("IntLit", (yylsp[0]).first_line, (yylsp[0]).first_column, 1, (yyvsp[0].intlit));}
#line 3010 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 1194 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("RealLit", (yylsp[0]).first_line, (yylsp[0]).first_column, 1, (yyvsp[0].reallit));}
#line 3016 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1196 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("ParamList", -1, -1, 0, 2, (yyvsp[-2].node), (yyvsp[-1].node));}
#line 3022 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 1198 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("CommaExprAux", -1, -1, 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 3028 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1199 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", -1, -1, 0, NULL);}
#line 3034 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1201 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Id", (yylsp[0]).first_line, (yylsp[0]).first_column, 1, (yyvsp[0].id));}
#line 3040 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1203 "mpacompile.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("String", (yylsp[0]).first_line, (yylsp[0]).first_column, 1, (yyvsp[0].string));}
#line 3046 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3050 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1205 "mpacompile.y" /* yacc.c:1906  */

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
		generateCode();
	}
	return 0;
}
