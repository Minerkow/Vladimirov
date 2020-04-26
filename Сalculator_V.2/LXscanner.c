//
// Created by bibi on 18.03.2020.
//

#include "Calculator.h"

static void swap_lexems(struct lexem_t* lexem1, struct lexem_t* lexem2)
{
    struct lexem_t copy = *lexem1;
    *lexem1 = *lexem2;
    *lexem2 = copy;
}

struct lex_array_t lex_string(const char *str) {
    assert(str != NULL);

    struct lex_array_t larr = { (struct lexem_t*)calloc(ICAP , sizeof(struct lexem_t)),
                                0,
                                ICAP };
    if (larr.lexems == NULL)
        exit(2);
    assert(larr.lexems != NULL);

    int i = 0;
    struct variable_t* hash_table = create_hash_table();
    variable_value(0, 0, false, hash_table);

    while (str[i])
    {
        if (larr.size == larr.capacity ) {
            larr.lexems = (struct lexem_t *)realloc(larr.lexems, sizeof(struct lexem_t) * (larr.capacity + 10));
            assert(larr.lexems);
            larr.capacity += 10;
        }

        //Read Space

        if (isspace(str[i]))
        {
            i++;
            continue;
        }

        //Read Numbers

        if (isdigit(str[i]))
        {
            larr.lexems[larr.size].kind = NUM;
            larr.lexems[larr.size].lex.num = 0;
            while(isdigit(str[i]))
            {
                larr.lexems[larr.size].lex.num = larr.lexems[larr.size].lex.num * 10 + str[i] - '0';
                i++;
            }
            larr.size++;
            continue;
        }

        //Read Operation

        if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
        {
            larr.lexems[larr.size].kind = OP;
            int code = str[i];
            switch ( code )
            {
                case '+':
                    larr.lexems[larr.size].lex.op = ADD;
                    break;
                case '-':
                    larr.lexems[larr.size].lex.op = SUB;
                    break;
                case '*':
                    larr.lexems[larr.size].lex.op = MUL;
                    break;
                case '/':
                    larr.lexems[larr.size].lex.op = DIV;
                default:;
            }
            larr.size++;
            i++;
            continue;
        }

        //Read Brace

        if (str[i] == '(' || str[i] == ')')
        {
            larr.lexems[larr.size].kind = BRACE;
            int code = str[i];
            switch (code)
            {
                case '(':
                    larr.lexems[larr.size].lex.b = LBRAC;
                    break;
                case ')':
                    larr.lexems[larr.size].lex.b = RBRAC;
                    break;
                default:;
            }
            larr.size++;
            i++;
            continue;
        }

        //Read Variables and Command
        if (isalpha(str[i]))
        {
            int numsymbol = 0;
            int hash = 0;
            larr.lexems[larr.size].kind = VARIABLE;
            char* word = (char*)calloc(MAXLENWORD + 1, sizeof(char));
            while (isalpha(str[i]))
            {
                hash = calc_hash(hash, numsymbol, str[i]);
                if (numsymbol == MAXLENWORD - 1)
                {
                    printf("Easy, too many letters in your variable");
                    exit(31);
                }
                word[numsymbol] = str[i];
                numsymbol++;
                i++;
            }
            if (!strcmp(word, "print"))
            {
                larr.lexems[larr.size].kind = COMMAND;
                larr.lexems[larr.size].lex.com = PRINT;
                larr.size++;
                continue;
            }
            larr.lexems[larr.size].lex.num = check_hash_table(hash_table, hash, word);
            larr.size++;
            continue;
        }

        //Read Special Characters
        if (str[i] == '?')
        {
            larr.lexems[larr.size].kind = COMMAND;
            larr.lexems[larr.size].lex.com = INPUT;
            larr.size++;
            i++;
            continue;
        }

        if (str[i] == '=')
        {
            larr.lexems[larr.size].kind = COMMAND;
            larr.lexems[larr.size].lex.com = ASSIGN;
            larr.size++;
            i++;
            continue;
        }

        if (str[i] == ';')
        {
            larr.lexems[larr.size].kind = COMMAND;
            larr.lexems[larr.size].lex.com = END_COMMAND;
            larr.size++;
            i++;
            continue;
        }
    }

    //free_hash_table(hash_table);
    return larr;
}

void
print_op(enum operation_t opcode) {
    switch(opcode) {
        case ADD: printf(" PLUS"); break;
        case SUB: printf(" MINUS"); break;
        case MUL: printf(" MUL"); break;
        case DIV: printf(" DIV"); break;
        default: assert(0 && "unknown opcode");
    }
}
void
print_brace(enum braces_t bracetype){
    switch(bracetype) {
        case LBRAC: printf(" LBRAC"); break;
        case RBRAC: printf(" RBRAC"); break;
        default: assert(0 && "unknown bracket");
    }
}


void
print_num(int n) {
    printf(" NUMBER:%d", n);
}

void
print_variable(int n){
    printf(" VARIABLE:%d", n);
}

void
print_command(enum command_t command){
    switch(command){
        case INPUT: printf(" INPUT"); break;
        case PRINT: printf(" PRINT"); break;
        case ASSIGN: printf(" ASSIGN"); break;
        case END_COMMAND: printf(" END_COMMAND"); break;
        default: assert(0 && "unknown command");
    }
}

void
print_lexem(struct lexem_t lxm) {
    switch(lxm.kind) {
        case OP: print_op(lxm.lex.op); break;
        case BRACE: print_brace(lxm.lex.b); break;
        case NUM: print_num(lxm.lex.num); break;
        case VARIABLE: print_variable(lxm.lex.num); break;
        case COMMAND: print_command(lxm.lex.com); break;
        default: assert(0 && "unknown lexem");
    }
}

void dump_lexarray(struct lex_array_t pl) {
    int i;
    assert(pl.lexems != NULL);
    for (i = 0; i < pl.size; ++i)
        print_lexem(pl.lexems[i]);
}

