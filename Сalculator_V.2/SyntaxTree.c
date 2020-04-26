
#include "HeaderLX.h"

struct variable_t {
    int value;
    int hash;
    struct variable_t* next;
};

static struct lexem_t get_cur_lexem (int i, struct lex_array_t* ptr);
static int get_cur_size(struct lex_array_t* ptr);
static int is_add_sub (int i);
static int is_mul_div(int i);
static int is_brace(int i);
static int is_end_command(struct lexem_t lexem);
static void assign_analyzer(int* i, struct lex_array_t larr);
static int is_input(struct lexem_t lexem);
static void update_variables(int variable, int value, struct variable_t* top);
static void print_list(struct variable_t *top);
static int find_variable(int variable, struct variable_t* ptr);
static void print_analyzer(int*i, struct lex_array_t larr);
static void free_list(struct variable_t *top);


//Arithmetic sentence analysis
//! @param[in] pointer to an array of tokens
//! @return void

void sentence_analyzer(struct lex_array_t larr)
{
    int i = 0;
    struct variable_t* top_vareable = (struct variable_t*)calloc(1, sizeof(struct variable_t));
    update_variables(0, 0, top_vareable);
    find_variable(0, top_vareable);
    while (i != larr.size)
    {
        if (larr.lexems[i].kind != COMMAND)
        {
            printf("Expected command");
            exit(33);
        }
        switch (larr.lexems[i].lex.com)
        {
            case ASSIGN :
                assign_analyzer(&i, larr);
                //printf("Assign analyzer");
                break;
            case INPUT :
                printf("It seems there should not be ?");
                exit(34);
            case PRINT :
                print_analyzer(&i, larr);
                break;
            case END_COMMAND :
                printf("It seems there should not be ;");
                exit(35);
            default: {
                printf("Syntax ERROR");
                exit(36);
            }
        }
    }
    free_list(top_vareable);
    //print_list(top_vareable);
}

//Function parses an assignment token
//! @param[in] pointer to the item number in the token array
//! @param[in] pointer to an array of tokens
//! @return void

static void assign_analyzer(int* i, struct lex_array_t larr)
{
    (*i)++;
    int variable = 0;
    int value = 0;
    if (larr.lexems[*i].kind == VARIABLE)
    {
        variable = larr.lexems[*i].lex.num;
        (*i)++;
    }else{
        printf("Syntax ERROR");
        exit(37);
    }
    if (is_input(larr.lexems[*i]))
    {
        printf("Pls, enter value:");
        scanf("%d", &value);
        printf("\n");
        update_variables(variable, value, NULL);
        (*i)++;
    }else{
        //printf("{%d}\n", *i);
        value = BuildTree(larr, i);
        update_variables(variable, value, NULL);
        //printf("<%d>", *i);
        //printf("result = %d\n", value);
    }
    if (is_end_command(larr.lexems[*i])) {
        (*i)++;
    }else{
        printf("Expected ';'");
        exit(40);
    }
}

//Updates the list of variables
//! @param[in] variable hash
//! @param[in] variable value
//! @param[in] pointer to the top of the variable list
//! @return void

static void update_variables(int variable, int value, struct variable_t* top_variable)
{
    static struct variable_t* top = NULL;
    if (top_variable != NULL)
    {
        top = top_variable;
        top->hash = -1;
        //printf ("[%p]", top);
        return;
    }
    if (top->hash == -1)
    {
        top->hash = variable;
        top->value = value;
        //printf("{%d %d}", top->value, variable);
        return;
    }
    assert(top);
    struct variable_t* copy = top;
    while(copy != NULL)
    {
        //printf("<%d>", copy->hash);
        if (copy->hash == variable)
        {
            copy->value = value;
            //printf("pfff");
            return;
        }
        if (copy->next == NULL)
        {
            copy->next = (struct variable_t *) calloc(1, sizeof(struct variable_t));
            copy->next->value = value;
            //printf("[%d]", value);
            copy->next->hash = variable;
            return;
        }
        copy = copy->next;
    }
}

//Search for a variable in the variable list
//! @param[in] variable hash
//! @param[in] pointer to the top of the variable list
//! @return the value of a variable

static int find_variable(int variable, struct variable_t* ptr)
{
    static struct variable_t* top = NULL;
    if (ptr != NULL)
    {
        top = ptr;
        return 0;
    }
   // printf("_%d_\n", variable);
    //print_list(top);
    //printf("\n");
    struct variable_t* copy = top;
    while(copy != NULL)
    {
        if(copy->hash == variable) {
            //printf("-%d-", copy->hash);
            return copy->value;
        }
        copy = copy->next;
    }
    printf("Undeclared variable");
    exit(50);
}


//analyzes the print command
//! @param[in] pointer to the item number in the token array
//! @param[in] pointer to an array of tokens
//! @return void

static void print_analyzer(int*i, struct lex_array_t larr)
{
    (*i)++;
    int value = BuildTree(larr, i);
    (*i)++;
    printf("Result : %d\n", value);
}

//Prints a list of variables
//! @param[in] pointer to the top of the variable list
//! @return void

static void print_list(struct variable_t *top)
{
    while(top != NULL)
    {
        printf("%d->", top->value);
        struct variable_t* tmp = top->next;
        top = tmp;
    }
}

static int is_input(struct lexem_t lexem)
{
    return (lexem.kind == COMMAND && lexem.lex.com == INPUT);
}

static int is_end_command(struct lexem_t lexem)
{
    return (lexem.kind == COMMAND && lexem.lex.com == END_COMMAND);
}


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

//Builds an expression tree and returns the result
//! @param[in] pointer to an array of tokens
//! @param[in] pointer to the item number in the token array
//! @return value of expression

int BuildTree (struct lex_array_t larr, int* j)
{
    int i = *j;
    get_cur_lexem(0, &larr);
    get_cur_size(&larr);
    struct node_t* top = Expr(&i);
    if (top == NULL)
    {
        printf ("Empty input");
        exit(1);
    }
    *j = i;
    int res = calc_result(top);
    free_tree(top);
    return res;
    //print_tree(top);
    //printf("%d", calc_result(top));
    //return top;
}

//Calc tree expression
//! @param[in] pointer to the top of the tree
//! @return value of expression

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

//Builds leaves of an expression tree with addition and subtraction operations
//! @param[in] pointer to the item number in the token array
//! @return pointer to a tree leaf

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

//Builds leaves of the expression tree with operations of multiplication and division
//! @param[in] pointer to the item number in the token array
//! @return pointer to a tree leaf

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

//Builds the leaves of an expression tree with numbers and variables, and also analyzes the brackets
//! @param[in] pointer to the item number in the token array
//! @return pointer to a tree leaf

struct node_t* Term (int* i)
{
    struct node_t* node = NULL;
    if (get_cur_lexem(*i, NULL).kind == VARIABLE)
    {
        node = Create_Node();
        node->lexem.kind = NUM;
        node->lexem.lex.num = find_variable(get_cur_lexem(*i, NULL).lex.num, NULL);
        (*i)++;
        return node;
    }
    if (get_cur_lexem(*i, NULL).kind == NUM)
    {
        node = Create_Node();
        node->lexem = get_cur_lexem(*i, NULL);
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

//Creates a tree node
//! @return pointer to node

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

//Prints the contents of the node
//! @param[in] token
//! @return void

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
            exit(10);
    }
}

//Prints the contents of the tree
//! @param[in] pointer to the top of the tree
//! @return void

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
}

static void free_list(struct variable_t *top)
{
    while(top != NULL)
    {
        struct variable_t* tmp = top->next;
        free(top);
        top = tmp;
    }
}
