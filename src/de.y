/** Grammar for dice expression.
 * S       ::= expr
 * expr    ::= integer | '-' expr | expr '-' expr | expr '+' expr | [integer] 'd' integer ignore
 * ignore  ::= ('<' | '>' [integer])*
 * integer ::= [0-9] | [1-9][0-9]+
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <assert.h>
#include "str.h"
#include "diceexpr.h"
// Call this on a parameter if compiler warns about unused parameter.
#define UNUSED_PARAM(x) ((void) (x))
int yylex();
void yyerror(const char *s);
// Set expr as input for lexer. Can't be NULL.
void set_scan_string(const char *expr);
// Free lexer's buffer.
void delete_buffer();
static enum parse_error roll(int nrolls,
                             int dice,
                             int ignore_small,
                             int ignore_large,
                             int *sum);
static int sort_ascending(const void *a, const void *b);
// Number of rolls for a dice.
static int nrolls;
// Number of smallest and largest rolls to ignore.
static int ignore_small, ignore_large;
// Dice expression after dices are rolled.
static str *rolled_expr;
// Evaluated result of a dice expression.
static int result;
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

%token INTEGER
%token INVALID_CHARACTER

%left '+' '-'
%nonassoc 'd'
%right '<' '>'

%nonassoc IGNORE_EMPTY
%nonassoc UMINUS

%%

program:
    expr    { result = $1; }
    ;

expr:
    INVALID_CHARACTER       {
                              PARSE_ERROR(DE_INVALID_CHARACTER);
                            }
    | INTEGER               {
                              if (str_append_format(rolled_expr, "%d", $1) != 0)
                                 PARSE_ERROR(DE_MEMORY);
                            }
    | '-' { if (str_append_char(rolled_expr, '-')) PARSE_ERROR(DE_MEMORY); } expr %prec UMINUS  { $$ = -$3; }
    | expr '-' { if (str_append_char(rolled_expr, '-')) PARSE_ERROR(DE_MEMORY);  } expr         { $$ = $1 - $4; }
    | expr '+' { if (str_append_char(rolled_expr, '+')) PARSE_ERROR(DE_MEMORY); }  expr         { $$ = $1 + $4; }
    | maybe_int 'd' INTEGER ignore_list     {
                                                int sum;
                                                enum parse_error e =
                                                    roll(nrolls, $3, ignore_small, ignore_large, &sum);
                                                if (e != 0)
                                                    PARSE_ERROR(e);
                                                $$ = sum;
                                                ignore_small = 0;
                                                ignore_large = 0;
                                            }
    ;

maybe_int:
    INTEGER     { nrolls = $1; }
    |           { nrolls = 1; }
    ;

ignore_list:
    ignore
    | ignore_list ignore
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
de_parse(const char *expr, int *value, char **rolled_expression) {
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
        rolled_expr = NULL;
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
roll(int nrolls, int dice, int ignore_small, int ignore_large, int *dice_sum) {
    if (nrolls <= 0)
        return DE_NROLLS;
    if (dice <= 0)
        return DE_DICE;
    if (ignore_small + ignore_large >= nrolls)
        return DE_IGNORE;

    int rolls[nrolls];

    for (int i = 0; i < nrolls; i++)
        rolls[i] = (int) (rand() / (double) RAND_MAX * dice + 1);

    qsort(rolls, nrolls, sizeof(int), sort_ascending);

    if (str_append_char(rolled_expr, '(') != 0)
        return DE_MEMORY;

    int sum = 0;
    int n = 0;
    for (int i = ignore_small; i < nrolls - ignore_large; i++) {
        sum += rolls[i];

        if (n > 0 && n < nrolls - ignore_large) {
            if (str_append_format(rolled_expr, "+%d", rolls[i]) != 0)
                return DE_MEMORY;
        }
        else {
            if (str_append_format(rolled_expr, "%d", rolls[i]) != 0)
                return DE_MEMORY;
        }
        n++;
    }

    if (str_append_char(rolled_expr, ')') != 0)
        return DE_MEMORY;

    *dice_sum = sum;

    return 0;
}

static int
sort_ascending(const void *a, const void *b) {
    const int *x = a;
    const int *y = b;

    if (*x < *y)  return -1;
    if (*x == *y) return 0;
    else          return 1;
}

// Empty, because on syntax error we don't want to print anything.
void
yyerror(const char *s) {
    UNUSED_PARAM(s);
}
