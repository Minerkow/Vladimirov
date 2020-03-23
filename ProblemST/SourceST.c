
//
// Created by Minerkow on 14.03.2020.
//

#include "HeaderLX.h"

struct node_t* BuildTree (struct lex_array_t larr)
{
    int i = 0;
    struct node_t* top = Expr(&larr, &i);
    printf("\n[%d]\n", calc_result(top));
    return top;
}

int calc_result(struct node_t *top)
{
    if (top->left == NULL && top->right == NULL)
        return top->lexem.lex.num;
    int l = calc_result(top->left);
    int r = calc_result(top->right);
    return calc(l, r, top);
}

int calc (int l, int r, struct node_t *top)
{
    if(top->lexem.kind == OP && top->lexem.lex.op == ADD)
        return l + r;
    if(top->lexem.kind == OP && top->lexem.lex.op == SUB)
        return l - r;
    if(top->lexem.kind == OP && top->lexem.lex.op == MUL)
        return l * r;
    if(top->lexem.kind == OP && top->lexem.lex.op == DIV)
        return l / r;
}

struct node_t* Expr(struct lex_array_t* larr, int* i)
{
    struct node_t* node = NULL;
    struct node_t* expr_left = Mult(larr, i);
    if (*i == larr->size-1)
        return expr_left;
    (*i)++;
    if (larr->lexems[*i].kind == OP) {
        if (larr->lexems[*i].lex.op == ADD || larr->lexems[*i].lex.op == SUB) {
            node = Create_Node();
            node->lexem = larr->lexems[*i];
            (*i)++;
            node->left = expr_left;
            node->right = Expr(larr, i);
            return node;
        }
        (*i)--;
    }
    return expr_left;
}

struct node_t* Mult(struct lex_array_t* larr, int* i)
{
    struct node_t* node = NULL;
    struct node_t* mult_left = Term(larr, i);
    if (*i == larr->size-1)
        return mult_left;
    (*i)++;
    if (larr->lexems[*i].kind == OP) {
        if (larr->lexems[*i].lex.op == MUL || larr->lexems[*i].lex.op == DIV) {
            node = Create_Node();
            node->lexem = larr->lexems[*i];
            (*i)++;
            node->left = mult_left;
            node->right = Mult(larr, i);
            return node;
        }
        (*i)--;
    }
    return mult_left;
}

struct node_t* Term(struct lex_array_t* larr, int* i)
{
    struct node_t* node = NULL;
    if (larr->lexems[*i].kind == NUM)
    {
        node = Create_Node();
        node->lexem = larr->lexems[*i];
        /*if (larr->size - 1 != (*i))
            (*i)++;*/
        return node;
    }
    if(larr->lexems[*i].lex.b == LBRAC && larr->lexems[*i].kind == BRACE)
    {
        (*i)++;
        node = Expr(larr, i);
        return node;
    }
    if (larr->lexems[*i].lex.b == RBRAC && larr->lexems[*i].kind == BRACE)
    {
        (*i)++;
        return node;
    }
    return 0;
}

struct node_t* Create_Node()
{
    struct node_t* node = (struct node_t*)calloc(1, sizeof(struct node_t));
    return node;
}

void free_tree(struct node_t *t) {
    if (t == NULL) return;
    free_tree(t->left);
    free_tree(t->right);
    free(t);
}

