/** Grammar for dice expression.
 * s      ::= expr
 * expr   ::= INTEGER | ('-'|'+') expr | expr '-' expr | expr '+' expr |
              [INTEGER] 'd' INTEGER ignore
 * ignore ::= ('<' | '>' [INTEGER])*
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <assert.h>
#include <inttypes.h>
#include "str.h"
#include "diceexpr.h"
// Call this on a parameter if compiler warns about unused parameter.
#define UNUSED_PARAM(x) ((void) (x))
int yylex();
void yyerror(const char *s);
// Set dice expression as input for lexer. Can't be NULL.
void set_scan_string(const char *expr);
// Free lexer's buffer.
void delete_buffer();
static enum parse_error roll(int_least64_t nrolls,
                             int_least64_t dice,
                             int_least64_t ignore_small,
                             int_least64_t ignore_large,
                             int_least64_t *sum);
static int sort_ascending(const void *a, const void *b);
// Number of smallest and largest rolls to ignore.
static int_least64_t ignore_small, ignore_large;
// Dice expression after dices are rolled.
static str *rolled_expr;
// Evaluated result of a dice expression.
static int_least64_t result;
// Parser error.
static enum parse_error parse_error;
/* Set parse_error type and stop yyparse().
 * @param type Parse error of type enum parse_error.
 */
#define PARSE_ERROR(type) do { \
    parse_error = type; \
    YYERROR; \
} while (0)
%}

%code requires { #define YYSTYPE int_least64_t }

%token INTEGER
%token INVALID_CHARACTER
%token OVERFLOW

%left '+' '-'
%nonassoc 'd'
%right '<' '>'

%nonassoc IGNORE_EMPTY
%nonassoc UMINUS UPLUS

%%

program:
    expr    { result = $1; }
    ;

expr:
    INVALID_CHARACTER       { PARSE_ERROR(DE_INVALID_CHARACTER); }
    | OVERFLOW              { PARSE_ERROR(DE_OVERFLOW); }
    | INTEGER               {
                              if (str_append_format(rolled_expr, "%" PRIdLEAST64, $1) != 0)
                                 PARSE_ERROR(DE_MEMORY);
                            }
    | '-' { if (str_append_char(rolled_expr, '-')) PARSE_ERROR(DE_MEMORY); } expr %prec UMINUS  { $$ = -$3; }
    | '+' { if (str_append_char(rolled_expr, '+')) PARSE_ERROR(DE_MEMORY); } expr %prec UPLUS   { $$ = $3; }
    | expr '-' { if (str_append_char(rolled_expr, '-')) PARSE_ERROR(DE_MEMORY);  } expr         { $$ = $1 - $4; }
    | expr '+' { if (str_append_char(rolled_expr, '+')) PARSE_ERROR(DE_MEMORY); }  expr         { $$ = $1 + $4; }
    | maybe_int 'd' INTEGER ignore_list     {
                                                int_least64_t sum;
                                                enum parse_error e =
                                                    roll($1, $3, ignore_small, ignore_large, &sum);
                                                if (e != 0)
                                                    PARSE_ERROR(e);
                                                $$ = sum;
                                                ignore_small = 0;
                                                ignore_large = 0;
                                            }
    ;

maybe_int:
    INTEGER     { $$ = $1; }
    |           { $$ = 1; }
    ;

ignore_list:
    ignore
    | ignore_list ignore
    // Should now be redundant.
    | %prec IGNORE_EMPTY
    ;

ignore:
    '<'                { ignore_small++; }
    | '>'              { ignore_large++; }
    | '<' INTEGER      { ignore_small += $2; } 
    | '>' INTEGER      { ignore_large += $2; } 
    ;

%%

enum parse_error
de_parse(const char *expr, int_least64_t *value, char **rolled_expression) {
    assert(expr != NULL);
    assert(*rolled_expression == NULL);

    if ((rolled_expr = str_new(NULL)) == NULL)
        return DE_MEMORY;

    enum parse_error retval = 0;

    set_scan_string(expr);
    int parse_retval = yyparse();
    // Any other error than bison's memory error.
    if (parse_retval == 1) {
        // If parse_error is set, then it's some other error than syntax error.
        retval = parse_error == 0 ? DE_SYNTAX_ERROR : parse_error;
        goto end;
    }
    else if (parse_retval == 2) {
        retval = DE_MEMORY;
        goto end;
    }
    *value = result;
    if (str_copy_to_chars(rolled_expr, rolled_expression) != 0)
        retval = DE_MEMORY;

    end:
        delete_buffer();
        str_free(rolled_expr);
        // Initialize all file globals for the next call.
        rolled_expr = NULL;
        ignore_small = 0;
        ignore_large = 0;
        parse_error = 0;
        result = 0;

    return retval;
}

/* Roll a dice.
 * Arguments must satisfy: ignore_small + ignore_large < nrolls.
 * @param nrolls Number of rolls for a dice. Must be > 0.
 * @param dice Number of sides in a dice. Must be > 0.
 * @param ignore_small Ignore this many smallest rolls.
 * @param ignore_large Ignore this many largest rolls.
 * @param dice_sum Sum of dices rolled.
 * @return Zero on success, enum parse_error otherwise.
 */
static enum parse_error
roll(int_least64_t nrolls,
     int_least64_t dice,
     int_least64_t ignore_small,
     int_least64_t ignore_large,
     int_least64_t *dice_sum) {
    if (nrolls <= 0)
        return DE_NROLLS;
    if (dice <= 0)
        return DE_DICE;
    if (ignore_small + ignore_large >= nrolls)
        return DE_IGNORE;

    int_least64_t rolls[nrolls];

    for (int_least64_t i = 0; i < nrolls; i++)
        rolls[i] = (int_least64_t) (rand() / (double) RAND_MAX * dice + 1);

    qsort(rolls, nrolls, sizeof(int_least64_t), sort_ascending);

    if (str_append_char(rolled_expr, '(') != 0)
        return DE_MEMORY;

    int_least64_t sum = 0;
    int_least64_t nth_included_roll = 0;
    for (int_least64_t i = ignore_small; i < nrolls - ignore_large;
         i++, nth_included_roll++) {
        sum += rolls[i];

        const char *format_with_plus_or_not =
            nth_included_roll > 0 && nth_included_roll < nrolls - ignore_large ?
                "+%" PRIdLEAST64 : "%" PRIdLEAST64;
        if (str_append_format(rolled_expr, format_with_plus_or_not, rolls[i]) != 0)
            return DE_MEMORY;
    }

    if (str_append_char(rolled_expr, ')') != 0)
        return DE_MEMORY;

    *dice_sum = sum;

    return 0;
}

static int
sort_ascending(const void *a, const void *b) {
    const int_least64_t *x = a;
    const int_least64_t *y = b;

    if (*x < *y)  return -1;
    if (*x == *y) return 0;
    else          return 1;
}

// Empty, because on syntax error we don't want to print anything.
void
yyerror(const char *s) {
    UNUSED_PARAM(s);
}
