#ifndef PROBLEMLX_HEADERLX_H
#define PROBLEMLX_HEADERLX_H

#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//LX
// lexem is operation | brace | number
enum lexem_kind_t {  OP , BRACE, NUM = 228, VOID};

// operation is: +, -, *, /
enum operation_t { ADD, SUB, MUL, DIV };

// braces are: (, )
enum braces_t {  LBRAC , RBRAC };

// lexem is one of lexem kind entities
// if (l.kind == BRACE)
//   assert(l.lex.b == LBRAC || l.lex.b == RBRAC)
struct lexem_t {
    enum lexem_kind_t kind;
    union {
        enum operation_t op;
        enum braces_t b;
        int num;
    } lex;
};

// array of lexems
// size: actual number of elements
// capacity: number of additions before realloc
    struct lex_array_t {
    struct lexem_t *lexems;
    int size, capacity;
};

// initial capacity
enum { ICAP = 10 };

// string to lexem array: see Problem LX on slides
struct lex_array_t lex_string(const char *str);

// printing
void print_lexem(struct lexem_t lxm);
void dump_lexarray(struct lex_array_t pl);

//Syntax Tree
struct node_t
{
    struct lexem_t lexem;
    struct node_t* right;
    struct node_t* left;
};

struct node_t* Expr(int* i);
struct node_t* Mult(int* i);
struct node_t* Term(int* i);
struct node_t* Create_Node();
int calc_result(struct node_t *top);
int calc (int l, int r, struct node_t *top);
struct node_t* BuildTree (struct lex_array_t larr);


void print_node (struct lexem_t lex);
void print_tree (struct node_t* top);
#endif //PROBLEMST_HEADERST_H


//expr ::= mult {+, -} expr | mult
//mult ::= term {*, /} mult | term
//term ::= ( expr ) | number





void print_node (struct lexem_t lex);
void print_tree (struct node_t* top);
