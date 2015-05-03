/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "mpasemantic.y" /* yacc.c:339  */

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
		symbol_col = n->children[2]->col;
		/*if (!check_symbol_type(symbol_type)) {
			printf("Line %d, col %d: Type identifier expected\n", symbol_line, symbol_col);
			exit(0);
		}*/
		insert_symbol(table[2], n->children[0]->value, "function", NULL, NULL);
		while(table[cur_table_index] != NULL) {
			cur_table_index++;
		}
		table[cur_table_index] = new_symbol_table("Function");
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
		/*if (!check_symbol_type(symbol_type)) {
			printf("Line %d, col %d: Type identifier expected\n", symbol_line, symbol_col);
			exit(0);
		}*/
		for (i=0;i<n->n_children-1;i++) {
			insert_symbol(table[cur_table_index], n->children[i]->value, symbol_type, "param", NULL);
		}
	}
	else if (!strcmp(n->type, "VarParams")) {
		symbol_type = n->children[n->n_children-1]->value;
		symbol_line = n->children[n->n_children-1]->line;
		symbol_col = n->children[n->n_children-1]->col;
		/*if (!check_symbol_type(symbol_type)) {
			printf("Line %d, col %d: Type identifier expected\n", symbol_line, symbol_col);
			exit(0);
		}*/
		for (i=0;i<n->n_children-1;i++) {
			insert_symbol(table[cur_table_index], n->children[i]->value, symbol_type, "varparam", NULL);
		}
	}
	for (i=0;i<n->n_children;i++) {
		build_table(n->children[i]);
	}
}


#line 514 "y.tab.c" /* yacc.c:339  */

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
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 456 "mpasemantic.y" /* yacc.c:355  */

	char *intlit, *reallit, *string, *id;
	struct node *node;

#line 637 "y.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 652 "y.tab.c" /* yacc.c:358  */

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
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
       0,   469,   469,   471,   473,   475,   476,   478,   479,   481,
     483,   485,   486,   488,   489,   491,   492,   493,   496,   498,
     499,   501,   503,   505,   506,   508,   509,   511,   513,   515,
     517,   519,   521,   523,   524,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   537,   538,   540,   541,   542,
     544,   545,   546,   547,   548,   549,   550,   552,   553,   555,
     556,   557,   558,   559,   561,   562,   563,   564,   565,   566,
     568,   569,   570,   571,   572,   573,   575,   577,   578,   580,
     582
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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
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

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

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
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 469 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node) = parsing_tree = create_node("Program", 1, 2, (yyvsp[-3].node), (yyvsp[-1].node));}
#line 1846 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 471 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("ProgHeading", 0, 1, (yyvsp[-3].node));}
#line 1852 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 473 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("ProgHeading", 0, 3, (yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));}
#line 1858 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 475 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("VarPart", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 1864 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 476 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("VarPart", yylineno, col-yyleng, 1, NULL);}
#line 1870 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 478 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("VarPartAux", 0, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 1876 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 479 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", 0, 0, 0, NULL);}
#line 1882 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 481 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("VarDecl", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 1888 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 483 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("IDList", 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 1894 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 485 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("IDListAux", 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 1900 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 486 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", 0, 0, 0, NULL);}
#line 1906 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 488 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncPart", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 1912 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 489 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("FuncPart", yylineno, col-yyleng, 1, NULL);}
#line 1918 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 491 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncDecl", 1, 1, (yyvsp[-2].node));}
#line 1924 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 492 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncDef2", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 1930 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 493 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncDef", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 1936 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 496 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncHeading", 0, 3, (yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[0].node));}
#line 1942 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 498 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncHeadingAux", 0, 1, (yyvsp[0].node));}
#line 1948 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 499 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("FuncParams", yylineno, col-yyleng, 1, NULL);}
#line 1954 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 501 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncIdent", 0, 1, (yyvsp[0].node));}
#line 1960 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 503 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FuncParams", 1, 2, (yyvsp[-2].node), (yyvsp[-1].node));}
#line 1966 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 505 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("FormalParamListAux", 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 1972 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 506 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", 0, 0, 0, NULL);}
#line 1978 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 508 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node) = create_node("FormalParams", 0, 1, (yyvsp[0].node));}
#line 1984 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 509 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node) = create_node("FormalParams", 0, 1, (yyvsp[0].node));}
#line 1990 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 511 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("VarParams", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 1996 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 513 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Params", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2002 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 515 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_funcblock((yyvsp[-1].node), (yyvsp[0].node));}
#line 2008 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 517 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("StatPart", 0, 1, (yyvsp[0].node));}
#line 2014 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 519 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("CompStat", 0, 1, (yyvsp[-1].node));}
#line 2020 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 521 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("StatList", 1, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2026 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 523 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("SemicStatAux", 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2032 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 524 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", 0, 0, 0, NULL);}
#line 2038 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 526 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Stat", 0, 1, (yyvsp[0].node));}
#line 2044 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 527 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_ifelse((yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));}
#line 2050 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 528 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_ifelse((yyvsp[-2].node), (yyvsp[0].node), create_terminal("StatList", yylineno, col-yyleng, 1, NULL));}
#line 2056 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 529 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_while((yyvsp[-2].node), (yyvsp[0].node));}
#line 2062 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 530 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_repeat((yyvsp[-2].node), (yyvsp[0].node));}
#line 2068 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 531 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("ValParam", 1, 2, (yyvsp[-4].node), (yyvsp[-1].node));}
#line 2074 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 532 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Assign", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2080 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 533 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("WriteLn", 1, 1, (yyvsp[0].node));}
#line 2086 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 534 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("WriteLn", yylineno, col-yyleng, 1, (yyvsp[0].string));}
#line 2092 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 535 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", 0, 0, 0, NULL);}
#line 2098 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 537 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("WritelnPList", 0, 2, (yyvsp[-2].node), (yyvsp[-1].node));}
#line 2104 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 538 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("WritelnPList", 0, 2, (yyvsp[-2].node), (yyvsp[-1].node));}
#line 2110 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 540 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("CommaExpStrAux", 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2116 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 541 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("CommaExpStrAux", 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2122 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 542 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", 0, 0, 0, NULL);}
#line 2128 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 544 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Eq", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2134 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 545 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Neq", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2140 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 546 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Lt", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2146 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 547 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Gt", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2152 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 548 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Leq", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2158 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 549 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Geq", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2164 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 550 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("SimpleExpr", 0, 1, (yyvsp[0].node));}
#line 2170 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 552 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Term", 0, 1, (yyvsp[0].node));}
#line 2176 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 553 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("AddOP", 0, 1, (yyvsp[0].node));}
#line 2182 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 555 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Add", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2188 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 556 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Sub", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2194 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 557 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Or", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2200 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 558 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Minus", 1, 1, (yyvsp[0].node));}
#line 2206 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 559 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Plus", 1, 1, (yyvsp[0].node));}
#line 2212 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 561 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("RealDiv", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2218 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 562 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Mul", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2224 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 563 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("And", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2230 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 564 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Div", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2236 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 565 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Mod", 1, 2, (yyvsp[-2].node), (yyvsp[0].node));}
#line 2242 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 566 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Factor", 0, 1, (yyvsp[0].node));}
#line 2248 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 568 "mpasemantic.y" /* yacc.c:1646  */
    {;}
#line 2254 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 569 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Not", 1, 1, (yyvsp[0].node));}
#line 2260 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 570 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("LbracRbrac", 0, 1, (yyvsp[-1].node));}
#line 2266 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 571 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("Call", 1, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2272 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 572 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("IntLit", yylineno, col-yyleng, 1, (yyvsp[0].intlit));}
#line 2278 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 573 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("RealLit", yylineno, col-yyleng, 1, (yyvsp[0].reallit));}
#line 2284 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 575 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("ParamList", 0, 2, (yyvsp[-2].node), (yyvsp[-1].node));}
#line 2290 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 577 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_node("CommaExprAux", 0, 2, (yyvsp[-1].node), (yyvsp[0].node));}
#line 2296 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 578 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Empty", yylineno, col-yyleng, 0, NULL);}
#line 2302 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 580 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("Id", yylineno, col-yyleng, 1, (yyvsp[0].id));}
#line 2308 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 582 "mpasemantic.y" /* yacc.c:1646  */
    {(yyval.node)=create_terminal("String", yylineno, col-yyleng, 1, (yyvsp[0].string));}
#line 2314 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2318 "y.tab.c" /* yacc.c:1646  */
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
                      yytoken, &yylval);
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


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
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
#line 584 "mpasemantic.y" /* yacc.c:1906  */

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
