//
// Created by bibi on 24.04.2020.
//

#include "Header.h"

void Random_Sentence()
{
    struct var_t* vars = (struct var_t*)calloc(1, sizeof(struct var_t));
    int num_without_var = 1 + rand() % 3;
    for (int i = 0; i < num_without_var; ++i)
    {
        struct lexem_t* top =  Random_Variable(vars);
        Print_Expr(top, vars, false);
    }
    int num_with_var = rand() % 6;
    for (int i = 0; i < num_with_var; ++i)
    {
        int r = rand() % 2;
        if (r == 0)
        {
            struct lexem_t* top =  Random_Variable(vars);
            Print_Expr(top, vars, true);
        }
        if (r)
        {
            struct lexem_t* top = Random_From_Var_List(vars);
            Print_Expr(top, vars, true);
        }
    }
    Print_All_Var(vars);
    free_varlist(vars);
}

void Print_Expr(struct lexem_t* top, struct var_t* vars, bool var_on)
{
    top->next = Create_Lexem();
    top->next->kind = COMMAND;
    top->next->lex.com = ASSIGN;
    top->next->next = Random_Expression(vars, var_on);
    print_list(top);
    printf(";\n");
    free_exprlist(top);
}

struct lexem_t* Random_Expression(struct var_t* vars, bool var_on)
{
    int num_signs = 1 + rand() % 20;
    struct lexem_t* top = Create_Exprlist(num_signs, vars, var_on);
    return top;
}

struct lexem_t* Create_Exprlist(int num, struct var_t* vars, bool var_on)
{
    struct lexem_t* top = Create_Lexem();
    top->kind = NUM;
    top->lex.num = rand() % 1000;
    struct lexem_t* copy = top;
    //print_node(copy);
    for (int i = 1; i < 2*num + 1 ; ++i)
    {
        if (i % 2 == 0) {
            copy->next = Create_Lexem();
            int r = rand() % 4;
            if (r == 0 && var_on == true) {
                copy->next->kind = VARIABLE;
                copy->next = Random_From_Var_List(vars);
                copy = copy->next;
            } else {
                copy->next->kind = NUM;
                copy->next->lex.num = 1 + rand() % 1000;
                copy = copy->next;
            }
        }
        if (i % 2 == 1)
        {
            copy->next = Create_Lexem();
            copy->next->kind = OP;
            copy->next->lex.op = rand() % 4;
            copy = copy->next;
            //print_node(copy);
        }
    }
    int num_brace = rand() % num;
    for (int i = 0; i < num_brace; ++i)
        Set_Brace(top);
    return top;
}

void Set_Brace(struct lexem_t* top)
{
    bool LBRAC = false;
    while(top->next != NULL)
    {
        int r = rand() % 4;
        if(r == 0 && top->next->kind == NUM)
        {
            struct lexem_t* brace = Create_Lexem();
            brace->kind = BRACE;
            brace->lex.b = LBRAC;
            struct lexem_t* ptr_copy = top->next;
            top->next = brace;
            brace->next = ptr_copy;
            LBRAC = true;
            break;
        }
        top = top->next;
    }
    if (LBRAC) {
        while (top->next != NULL) {
            int r = rand() % 3;
            if (r == 0 && top->next->kind == NUM && top->kind != BRACE) {
                struct lexem_t *brace = Create_Lexem();
                brace->kind = BRACE;
                brace->lex.b = RBRAC;
                struct lexem_t *ptr_copy = top->next->next;
                top->next->next = brace;
                brace->next = ptr_copy;
                return;
            }
            top = top->next;
        }
        struct lexem_t *brace = Create_Lexem();
        brace->kind = BRACE;
        brace->lex.b = RBRAC;
        top->next = brace;
    }
}

struct lexem_t* Create_Lexem()
{
    struct lexem_t* top = (struct lexem_t*)calloc(1, sizeof(struct lexem_t));
    assert(top);
    return top;
}

void print_list(struct lexem_t *top)
{
    if (top == NULL)
        return;
    print_node_2(top);
    //printf("->");
    print_list(top->next);
}

struct lexem_t* Random_Variable(struct var_t* vars)
{
    int len_variable = 1 + random() % 4;
    char* string = (char*)calloc(len_variable + 1, sizeof(char));
    for (int i = 0; i < len_variable; ++i)
    {
        string[i] = 'a' + random() % 26;
    }
    struct lexem_t* var = Create_Lexem();
    var->kind = VARIABLE;
    var->lex.variable = string;

    if (vars->variable == NULL)
        vars->variable = string;
    else {
        while(vars->next)
            vars = vars->next;
        vars->next = (struct var_t*)calloc(1, sizeof(struct var_t));
        vars->next->variable = string;
    }
    return var;
}

struct lexem_t* Random_From_Var_List(struct var_t* vars)
{
    struct var_t* copy_vars = vars;
    struct lexem_t* res = Create_Lexem();
    res->kind = VARIABLE;
    while (vars->next)
    {
        int r = rand() % 3;
        {
            res->lex.variable = vars->variable;
            return res;
        }
        vars = vars->next;
    }
    res->lex.variable = copy_vars->variable;
    return res;
}

void Print_All_Var(struct var_t* vars)
{
    while (vars->next)
    {
        printf("print %s", vars->variable);
        printf(";\n");
        vars = vars->next;
    }
    printf("print %s", vars->variable);
    printf(";\n");
}

void free_varlist(struct var_t *top)
{
    while(top != NULL)
    {
        struct var_t* tmp = top->next;
        free(top);
        top = tmp;
    }
}
void free_exprlist(struct lexem_t *top)
{
    while(top != NULL)
    {
        struct lexem_t* tmp = top->next;
        free(top);
        top = tmp;
    }
}
























void print_node (struct lexem_t* lex) {
    switch (lex->kind) {
        case OP:
            switch (lex->lex.op) {
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
            printf ("%d ", lex->lex.num);
            break;
        case VARIABLE:
            printf ("var_%d ", lex->lex.num);
            break;
        case COMMAND:
            switch (lex->lex.com){
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
        case BRACE:
            switch (lex->lex.b){
                case LBRAC:
                    printf("LBRAC ");
                    break;
                case RBRAC:
                    printf("RBRAC ");
                    break;
            }
            break;
        default:
            exit(2);
    }
}

void print_node_2 (struct lexem_t* lex) {
    switch (lex->kind) {
        case OP:
            switch (lex->lex.op) {
                case ADD:
                    printf ("+");
                    break;
                case SUB:
                    printf ("-");
                    break;
                case MUL:
                    printf ("*");
                    break;
                case DIV:
                    printf ("/");
                    break;
                default:
                    exit(1);
            }
            break;
        case NUM:
            printf ("%d ", lex->lex.num);
            break;
        case VARIABLE:
            printf ("%s ", lex->lex.variable);
            break;
        case COMMAND:
            switch (lex->lex.com){
                case PRINT:
                    printf("\nprint");
                    break;
                case INPUT:
                    printf(" ? ");
                    break;
                case ASSIGN:
                    printf(" = ");
                    break;
                case END_COMMAND:
                    printf(";");
                    break;
            }
            break;
        case BRACE:
            switch (lex->lex.b){
                case LBRAC:
                    printf("(");
                    break;
                case RBRAC:
                    printf(")");
                    break;
            }
            break;
        default:
            exit(2);
    }
}
