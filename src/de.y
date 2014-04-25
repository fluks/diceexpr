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
void yyerror(char *);
int yylex();
static int roll(int, int, int, int);
static int sort_ascending(const void *, const void *);
// Number of rolls for a dice.
static int nrolls;
// Number of smallest and largest rolls to ignore.
static int ignore_small, ignore_large;
// Dice expression after dices are rolled.
char rolled_expr[100];
// Pointer to next character in rolled_expr.
static char *re_char;
%}

%token INTEGER

%left '+' '-'
%nonassoc 'd'
%right '<' '>'

%nonassoc IGNORE_EMPTY
%nonassoc UMINUS

%%

program:
    program { re_char = rolled_expr; } expr '\n'   {
                                                     *++re_char = '\0';
                                                     printf("%s = %d\n", rolled_expr, $3);
                                                   }
    |
    ;

expr:
    INTEGER                                        {
                                                     int n = sprintf(re_char, "%d", $1);
                                                     re_char += n;
                                                   }
    | '-' { *re_char++ = '-'; } expr %prec UMINUS  { $$ = -$3; }
    | expr '-' { *re_char++ = '-'; } expr          { $$ = $1 - $4; }
    | expr '+' { *re_char++ = '+'; } expr          { $$ = $1 + $4; }
    | maybe_int 'd' INTEGER ignore_list {
        $$ = roll(nrolls, $3, ignore_small, ignore_large);
        ignore_small = 0;
        ignore_large = 0;
    }
    ;

maybe_int:
    INTEGER                                        { nrolls = $1; }
    |                                              { nrolls = 1; }
    ;

ignore_list:
    ignore
    | ignore_list ignore
    | %prec IGNORE_EMPTY
    ;

ignore:
    '<'                                            { ignore_small++; }
    | '>'                                          { ignore_large++; }
    | '<' INTEGER                                  { ignore_small += $2; } 
    | '>' INTEGER                                  { ignore_large += $2; } 
    ;

%%

int main() {
    srand(time(NULL));
    yyparse();
}

/** Roll a dice.
 * Arguments must satisfy: ignore_small + ignore_large < nrolls.
 * @param nrolls Number of rolls for a dice. Must be > 0.
 * @param dice Number of sides in a dice. Must be > 0.
 * @param ignore_small Ignore this many smallest rolls.
 * @param ignore_large Ignore this many largest rolls.
 * @return Sum of rolls.
 */
static int roll(int nrolls, int dice, int ignore_small, int ignore_large) {
    if (ignore_small + ignore_large >= nrolls)
        yyerror("Total number of ignores must be less than number of rolls");
    if (nrolls <= 0)
        yyerror("Number of rolls must be > 0");
    if (dice <= 0)
        yyerror("Number of sides in a dice must be > 0");


    int rolls[nrolls];

    for (int i = 0; i < nrolls; i++)
        rolls[i] = (int) (rand() / (double) RAND_MAX * dice + 1);

    qsort(rolls, nrolls, sizeof(int), sort_ascending);

    *re_char++ = '(';

    int sum = 0;
    int n = 0;
    for (int i = ignore_small; i < nrolls - ignore_large; i++) {
        sum += rolls[i];

        if (n > 0 && n < nrolls - ignore_large) {
            int nchars = sprintf(re_char, "+%d", rolls[i]);
            re_char += nchars;
        }
        else {
            int nchars = sprintf(re_char, "%d", rolls[i]);
            re_char += nchars;
        }
        n++;
    }

    *re_char++ = ')';

    return sum;
}

static int sort_ascending(const void *a, const void *b) {
    const int *x = a;
    const int *y = b;

    if (*x < *y)  return -1;
    if (*x == *y) return 0;
    else          return 1;
}

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}
