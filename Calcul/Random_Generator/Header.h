//
// Created by bibi on 24.04.2020.
//

#ifndef NEW_RANDOM_HEADER_H
#define NEW_RANDOM_HEADER_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

//LX
// lexem is operation | brace | number
enum lexem_kind_t {  OP , BRACE, NUM ,  VARIABLE, COMMAND};

// operation is: +, -, *, /
enum operation_t { ADD = 0, SUB = 1, MUL = 2, DIV = 3 };

// braces are: (, )
enum braces_t {  LBRAC , RBRAC };

//command is: print, ?, =, ;
enum command_t { PRINT, INPUT, ASSIGN, END_COMMAND};

// lexem is one of lexem kind entities
// if (l.kind == BRACE)
//   assert(l.lex.b == LBRAC || l.lex.b == RBRAC)
struct lexem_t {
    enum lexem_kind_t kind;
    union {
        enum operation_t op;
        enum braces_t b;
        enum command_t com;
        char* variable;
        int num;
    } lex;
    struct lexem_t* next;
};

// array of lexems
// size: actual number of elements
// capacity: number of additions before realloc
struct lex_array_t {
    struct lexem_t *lexems;
    int size, capacity;
};

struct var_t
{
    char* variable;
    struct var_t* next;
};

//initial sentence
enum {ISEN = 10};

// initial capacity
enum { ICAP = 10 };

// string to lexem array: see Problem LX on slides
struct lex_array_t lex_string(const char *str);

// printinvoid print_lexem(struct lexem_t lxm);
void dump_lexarray(struct lex_array_t pl);


void Random_Sentence();
struct lexem_t* Random_Expression(struct var_t* vars, bool var_on);
struct lexem_t* Random_Variable(struct var_t* vars);
struct lexem_t* Random_From_Var_List(struct var_t* vars);
struct lexem_t* Create_Exprlist(int num, struct var_t* vars, bool var_on);
struct lexem_t* Create_Lexem();
void Print_Expr(struct lexem_t* top, struct var_t* vars, bool var_on);
void Print_All_Var(struct var_t* vars);

void print_list(struct lexem_t* top);
void print_node (struct lexem_t* lex);
void print_node_2 (struct lexem_t* lex);
void Set_Brace(struct lexem_t* top);

void free_exprlist(struct lexem_t *top);
void free_varlist(struct var_t *top);

#endif //NEW_RANDOM_HEADER_H
