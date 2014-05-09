#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include "diceexpr.h"

int main() {
    srand(time(NULL));
    const size_t size = 100;
    char buf[size];
    int_least64_t result;
    char *rolled_expr = NULL;

    while (fgets(buf, size - 1, stdin) != NULL) {
        enum parse_error e = de_parse(buf, &result, &rolled_expr);
        if (e != 0) {
            switch (e) {
                case DE_MEMORY:            puts("oom"); break;
                case DE_INVALID_CHARACTER: puts("invalid character"); break;
                case DE_SYNTAX_ERROR:      puts("syntax error"); break;
                case DE_NROLLS:            puts("invalid number of rolls"); break;
                case DE_DICE:              puts("invalid number of dice sides"); break;
                case DE_IGNORE:            puts("invalid number of ignores"); break;
                case DE_OVERFLOW:          puts("integer overflow"); break;
                default:                   puts("unknown error");
            }
            continue;
        }
        printf("%s = %" PRIdLEAST64 "\n", rolled_expr, result);

        free(rolled_expr);
        rolled_expr = NULL;
    }
}
