//
// Created by Minerkow on 19.04.2020.
//

#include "Calculator.h"

static struct lexem_t get_cur_lexem (int i, struct lex_array_t* ptr);
static int get_cur_size(struct lex_array_t* ptr);
static bool is_add_sub (int i);
static bool is_mul_div(int i);
static int is_brace(int i);
static bool is_assign(int i);
static bool is_stop(int i);
static bool is_num_input_variable(int i);


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

static bool is_add_sub (int i)
{
    return (get_cur_lexem(i, NULL).lex.op == ADD || get_cur_lexem(i, NULL).lex.op == SUB);
}

static bool is_mul_div(int i)
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

static bool is_assign(int i)
{
    return (get_cur_lexem(i, NULL).kind == COMMAND && get_cur_lexem(i, NULL).lex.com == ASSIGN);
}

static bool is_stop(int i)
{
    struct lexem_t lexem = get_cur_lexem(i, NULL);
    return (lexem.kind == COMMAND && (lexem.lex.com == END_COMMAND || lexem.lex.com == ASSIGN));
}

static bool is_print(int i)
{
    struct lexem_t lexem = get_cur_lexem(i, NULL);
    return (lexem.kind == COMMAND && lexem.lex.com == PRINT);
}

static bool is_num_input_variable(int i)
{
    struct lexem_t lexem = get_cur_lexem(i, NULL);
    bool res = (lexem.kind == COMMAND && lexem.lex.com == INPUT)
                || (lexem.kind == VARIABLE)
                || (lexem.kind == NUM);
    return res;
}

void Analyzer (struct lex_array_t larr)
{
    int i = 0;
    while (i < larr.size)
    {
        BuildTree(larr, &i);
        i++;
    }
    free_all(larr);
}

void BuildTree (struct lex_array_t larr, int* j)
{
    int i = *j;
    get_cur_lexem(0, &larr);
    get_cur_size(&larr);
    struct node_t* top = Comm(&i);
    (*j) = i;
    if (top == NULL)
    {
        printf ("Empty input");
        exit(1);
    }
    calc_result(top);
    free_tree(top);
}

int calc_result(struct node_t *top)
{
    int l = 0;
    int r = 0;
    if (top->lexem.kind == COMMAND && top->lexem.lex.com == PRINT)
    {
        printf("Result: %d", calc_result(top->left));
        return 0;
    }
    if (top->lexem.kind == COMMAND && top->lexem.lex.com == ASSIGN)
    {
        if (top->left == NULL || top->right == NULL)
        {
            printf("ASSIGN Error");
            exit(103);
        }
        if (top->left->lexem.kind != VARIABLE)
        {
            printf("ASSIGN Error");
            exit(104);
        }
        int value = calc_result(top->right);
        variable_value(top->left->lexem.lex.num, value, true, NULL);
        return 0;
    }
    switch (top->lexem.kind)
    {
        case VARIABLE:
            return variable_value(top->lexem.lex.num, 0, false, NULL);
        case NUM:
            return top->lexem.lex.num;
        case COMMAND:
            if (top->lexem.lex.com == INPUT)
            {
                int input = 0;
                printf("pls, enter value: ");
                scanf("%d", &input);
                return input;
            }
            break;
        default:;
    }
    l = calc_result(top->left);
    r = calc_result(top->right);
    return calc(l, r, top);
}

int calc (int l, int r, struct node_t *top)
{
    switch (top->lexem.kind) {
        case OP:
            switch (top->lexem.lex.op){
                case ADD:
                    return l + r;
                case SUB:
                    return l - r;
                case MUL:
                    return l * r;
                case DIV:
                    return l / r;
                default:
                    printf("Error operation");
                    exit(101);
            }
        default:
            printf("Error");
            exit(102);
    }
}

struct node_t* Comm(int* i)
{
    struct node_t* node = NULL;
    struct node_t* comm_left = NULL;
    if (is_print(*i))
    {
        node = Create_Node();
        node->lexem = get_cur_lexem(*i, NULL);
        //print_node(get_cur_lexem(*i, NULL));
        (*i)++;
        node->left = Expr(i);
        node->right = NULL;
        return node;
    }
    comm_left = Expr(i);
    //print_node(comm_left->lexem);
    if (get_cur_lexem(*i, NULL).lex.com == END_COMMAND && get_cur_lexem(*i, NULL).kind == COMMAND)
        return comm_left;
    if(is_assign(*i)) {
        node = Create_Node();
        node->lexem = get_cur_lexem(*i, NULL);
        //print_node(node->lexem);
        (*i)++;
        //printf ("_%d_", (*i));
        node->left = comm_left;
        node->right = Expr(i);
        //print_node(node->right->lexem);
        return node;
    }
    return comm_left;
}

struct node_t* Expr(int* i)
{
    struct node_t* node = NULL;
    struct node_t* expr_left = Mult(i);
    if (get_cur_lexem(*i, NULL).lex.com == END_COMMAND && get_cur_lexem(*i, NULL).kind == COMMAND)
        return expr_left;

    while (is_add_sub(*i) && get_cur_lexem(*i, NULL).kind == OP)
    {
/*        if (*i >= get_cur_size(NULL) - 1)
            return expr_left;*/
        if (get_cur_lexem(*i, NULL).lex.com == END_COMMAND && get_cur_lexem(*i, NULL).kind == COMMAND)
            return expr_left;

        node = Create_Node();
        node->lexem = get_cur_lexem(*i, NULL);

        (*i)++;

        //printf("_%d_", get_cur_lexem(*i, NULL).kind);
        //printf("(%d)\n", *i);

        struct lexem_t lexem = get_cur_lexem(*i, NULL);
        if (lexem.kind != NUM && lexem.kind != BRACE && lexem.kind != VARIABLE)
        {
            printf("{%d}", *i);
            printf("Expected expression");
            exit(5);
        }
        node->left = expr_left;
        node->right = Mult(i);

        expr_left = node;
    }
    return expr_left;
}


struct node_t* Mult(int* i)
{
    struct node_t* node = NULL;
    struct node_t* mult_left = Term(i);
/*    if (*i >= get_cur_size(NULL) - 1)
        return mult_left;*/
    if (get_cur_lexem(*i, NULL).lex.com == END_COMMAND && get_cur_lexem(*i, NULL).kind == COMMAND)
        return mult_left;

    while (is_mul_div(*i) && get_cur_lexem(*i, NULL).kind == OP) {

        if (get_cur_lexem(*i, NULL).lex.com == END_COMMAND && get_cur_lexem(*i, NULL).kind == COMMAND)
            return mult_left;

        node = Create_Node();
        node->lexem = get_cur_lexem(*i, NULL);

        //printf("{%d}\n", *i);

        (*i)++;

        struct lexem_t lexem = get_cur_lexem(*i, NULL);
        if (lexem.kind != NUM && lexem.kind != BRACE && lexem.kind != VARIABLE)
        {
            printf("Expected expression");
            exit(5);
        }
        node->left = mult_left;
        node->right = Term(i);

        mult_left = node;
    }
    return mult_left;
}

struct node_t* Term (int* i)
{
    struct node_t* node = NULL;
    if (is_num_input_variable(*i))
    {
        node = Create_Node();
        node->lexem = get_cur_lexem(*i, NULL);
        if (get_cur_lexem(*i + 1, NULL).kind == NUM)
        {
            printf("Two numbers in a row");
            exit(9);
        }
        (*i)++;
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
        if (is_brace(*i) != RBRAC)
        {
            printf("Extra brace");
            exit(6);
        }
        (*i)++;

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
        case VARIABLE:
            printf ("var_%d ", lex.lex.num);
            break;
        case COMMAND:
            switch (lex.lex.com){
                case PRINT:
                    printf("PRINT ");
                    break;
                case INPUT:
                    printf("INPUT ");
                    break;
                case ASSIGN:
                    printf("ASSIGN ");
                    break;
                case END_COMMAND:
                    printf("END_COMMAND ");
                    break;
            }
            break;
        default:
            exit(2);
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
        printf ("NULL\n");
        return;
    }
    print_node (top->left->lexem);

    if (top->right == NULL)
    {
        printf("NULL ");
        return;
    }
    print_node (top->right->lexem);

    printf ("\n");

    print_tree (top->left);
    print_tree (top->right);
}

void free_all(struct lex_array_t larr)
{
    free(larr.lexems);
    variable_value(-1, 0, false, NULL);
}
