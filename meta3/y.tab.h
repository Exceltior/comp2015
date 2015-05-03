/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

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
#line 456 "mpasemantic.y" /* yacc.c:1909  */

	char *intlit, *reallit, *string, *id;
	struct node *node;

#line 137 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
