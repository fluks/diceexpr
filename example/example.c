#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "diceexpr.h"

int main() {
    srand(time(NULL));
    int_least64_t result;
    char *rolled_expr = NULL;
    char *line;

    // Don't complete filepaths.
    rl_bind_key('\t', NULL);

    while ((line = readline("> ")) != NULL) {
        enum parse_error e = de_parse(line, &result, &rolled_expr);
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
            goto clean_readline;
        }
        printf("%s = %" PRIdLEAST64 "\n", rolled_expr, result);

        add_history(line);

        free(rolled_expr);
        rolled_expr = NULL;
        clean_readline:
            free(line);
    }
}
