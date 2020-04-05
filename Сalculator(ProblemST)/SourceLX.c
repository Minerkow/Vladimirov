
#include "HeaderLX.h"

struct lex_array_t lex_string(const char *str) {
    assert(str != NULL);

    struct lex_array_t larr = { (struct lexem_t*)calloc(ICAP , sizeof(struct lexem_t)),
                                0,
                                ICAP };
    if (larr.lexems == NULL)
        exit(2);
    assert(larr.lexems != NULL);

    int i = 0;

    while (str[i])
    {
        if (larr.size == larr.capacity ) {
            larr.lexems = (struct lexem_t *)realloc(larr.lexems, sizeof(struct lexem_t) * (larr.capacity + 10));
            assert(larr.lexems);
            larr.capacity += 10;
        }
        if (isspace(str[i]))
        {
            i++;
            continue;
        }
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
    }
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
print_lexem(struct lexem_t lxm) {
    switch(lxm.kind) {
        case OP: print_op(lxm.lex.op); break;
        case BRACE: print_brace(lxm.lex.b); break;
        case NUM: print_num(lxm.lex.num); break;
        default: assert(0 && "unknown lexem");
    }
}

void dump_lexarray(struct lex_array_t pl) {
    int i;
    assert(pl.lexems != NULL);
    for (i = 0; i < pl.size; ++i)
        print_lexem(pl.lexems[i]);
}

