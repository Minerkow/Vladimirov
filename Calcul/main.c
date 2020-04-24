#include "Calculator.h"

enum { MAXLEN = 1024 };


int main() {
    //FILE* f = fopen("/home/bibi/CLionProject/Vladimirov/Calculator/test.expr", "r");
    //assert(f);
    int res = 0;
    struct lex_array_t larr = {};
    char inp[MAXLEN] = {0};

    res = scanf("%1023c", &inp);
    assert(res == 1);

    larr = lex_string(inp);

    if (larr.lexems == NULL) {
        printf("ERROR\n");
        return 0;
    }
    dump_lexarray(larr);
    printf("\n");
    Analyzer(larr);
    return 0;
}
