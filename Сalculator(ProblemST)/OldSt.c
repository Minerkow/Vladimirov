//
// Created by bibi on 18.03.2020.
//

#include "HeaderLX.h"

static struct lexem_t get_cur_lexem (int i, struct lex_array_t* ptr);
static int get_cur_size(struct lex_array_t* ptr);
static int is_add_sub (int i);
static int is_mul_div(int i);
static int is_brace(int i);

static struct lexem_t get_cur_lexem (int i, struct lex_array_t* ptr)
{
    static struct lex_array_t* larr = NULL;
    if (ptr != NULL)
        larr = ptr;
    return (larr->lexems[i]);
}

static int get_cur_size(struct lex_array_t* ptr)
{
    static struct lex_array_t* larr = NULL;
    if (ptr != NULL)
        larr = ptr;
    return (larr->size);
}

static int is_add_sub (int i)
{
    return (get_cur_lexem(i, NULL).lex.op == ADD || get_cur_lexem(i, NULL).lex.op == SUB);
}

static int is_mul_div(int i)
{
    return (get_cur_lexem(i, NULL).lex.op == MUL || get_cur_lexem(i, NULL).lex.op == DIV);
}

static int is_brace(int i)
{
    if (get_cur_lexem(i, NULL).lex.b == LBRAC && get_cur_lexem(i, NULL).kind == BRACE)
        return LBRAC;
    if (get_cur_lexem(i, NULL).lex.b == RBRAC && get_cur_lexem(i, NULL).kind == BRACE)
        return RBRAC;
    return 0;
}

struct node_t* BuildTree (struct lex_array_t larr)
{
    int i = 0;
    get_cur_lexem(0, &larr);
    get_cur_size(&larr);
    struct node_t* top = Expr(&i);
    if (top == NULL)
    {
        printf ("Empty input");
        exit(1);
    }
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

struct node_t* Expr(int* i)
{
    struct node_t* node = NULL;
    struct node_t* expr_left = Mult(i);
    if (*i == get_cur_size(NULL) - 1)
        return expr_left;

    if (get_cur_lexem(*i, NULL).kind == OP) {
        while (is_add_sub(*i)) {
            if (*i == get_cur_size(NULL) - 1)
                return expr_left;
            node = Create_Node();
            node->lexem = get_cur_lexem(*i, NULL);
            (*i)++;
            printf("{%d}\n", *i);
  /*          struct lexem_t lexem = get_cur_lexem(*i, NULL);
            if (lexem.kind != NUM && lexem.kind != BRACE)
            {
                printf("Expected expression");
                exit(5);
            }*/
            node->left = expr_left;
            node->right = Mult(i);
            expr_left = node;
        }
        return expr_left;
    }

    return expr_left;
}

struct node_t* Mult(int* i)
{
    struct node_t* node = NULL;
    struct node_t* mult_left = Term(i);
    if (*i == get_cur_size(NULL) - 1)
        return mult_left;
    (*i)++;
   // if (get_cur_lexem(*i, NULL).kind == OP) {
        if (is_mul_div(*i) && get_cur_lexem(*i, NULL).kind == OP) {
            node = Create_Node();
            node->lexem = get_cur_lexem(*i, NULL);
            (*i)++;
            struct lexem_t lexem = get_cur_lexem(*i, NULL);
            if (lexem.kind != NUM && lexem.kind != BRACE)
            {
                printf("Expected expression");
                exit(5);
            }
            node->left = mult_left;
            node->right = Term(i);
            return node;
        }
    return mult_left;
}

struct node_t* Term (int* i)
{
    struct node_t* node = NULL;
    if (get_cur_lexem(*i, NULL).kind == NUM)
    {
        node = Create_Node();
        node->lexem = get_cur_lexem(*i, NULL);
        if (get_cur_lexem(*i + 1, NULL).kind == NUM)
        {
            printf("Two numbers in a row");
            exit(9);
        }
        return node;
    }

    if(is_brace(*i) == LBRAC)
    {
        (*i)++;
        if (is_brace(*i) == RBRAC)
        {
            printf("Extra brace");
            exit(5);
        }
        node = Expr(i);
        if(is_brace(*i) != RBRAC)
        {
            printf("Extra brace");
            exit(6);
        }
        return node;
    }

    if (is_brace(*i) == RBRAC)
    {
        (*i)++;
        return node;
    }
    return 0;
}

struct node_t* Create_Node()
{
    struct node_t* node = (struct node_t*)calloc(1, sizeof(struct node_t));

    assert(node);

    return node;
}

void free_tree(struct node_t *t) {
    if (t == NULL) return;
    free_tree(t->left);
    free_tree(t->right);
    free(t);
}








/*
void print_node (struct lexem_t lex) {
    switch (lex.kind) {
        case OP:
            switch (lex.lex.op) {
                case ADD:
                    printf ("ADD ");
                    break;
                case SUB:
                    printf ("SUB ");
                    break;
                case MUL:
                    printf ("MUL ");
                    break;
                case DIV:
                    printf ("DIV ");
                    break;
                default:
                    exit(1);
            }
            break;
        case NUM:
            printf ("%d ", lex.lex.num);
            break;
        default:
            exit(1);
    }
}
void print_tree (struct node_t* top) {
    if (top == NULL){
        printf ("Error: top is NULL\n");
        return;
    }
    if (top->left == NULL && top->right == NULL)
        return;
    print_node (top->lexem);
    if (top->left == NULL) {
        printf ("\n");
        return;
    }
    print_node (top->left->lexem);
    if (top->right == NULL)
        return;
    print_node (top->right->lexem);
    printf ("\n");
    print_tree (top->left);
    print_tree (top->right);
}*/
