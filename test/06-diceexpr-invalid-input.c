#include "test.h"
#include "diceexpr.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static char *rolled_expr;
static int_least64_t value;
static enum parse_error error;
#define EXPR_SIZE 100
static char expr[EXPR_SIZE];

static void
setup() {
    rolled_expr = NULL;
    value = 0;
    error = 0;
}

static void
teardown() {
    free(rolled_expr);
}

START_TEST(bug_68020b7) {
    strcpy(expr, "1d1<");
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_IGNORE, "expr \"%s\" returns DE_IGNORE", expr);

    free(rolled_expr);
    rolled_expr = NULL;
    strcpy(expr, "1d1");
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == 0, "expr \"%s\" returns 0", expr);
}
END_TEST


START_TEST(invalid_character) {
    strcpy(expr, "a");
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_INVALID_CHARACTER,
        "expr \"%s\" returns DE_INVALID_CHARACTER", expr);
}
END_TEST

START_TEST(zero_nrolls) {
    strcpy(expr, "0d1");
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_NROLLS,
        "expr \"%s\" returns DE_NROLLS", expr);
}
END_TEST

START_TEST(zero_dice) {
    strcpy(expr, "d0");
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_DICE,
        "expr \"%s\" returns DE_DICE", expr);
}
END_TEST

START_TEST(ignore_small) {
    strcpy(expr, "1d1<");
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_IGNORE,
        "expr \"%s\" returns DE_IGNORE", expr);
}
END_TEST

START_TEST(ignore_large) {
    strcpy(expr, "2d2>3");
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_IGNORE,
        "expr \"%s\" returns DE_IGNORE", expr);
}
END_TEST

START_TEST(syntax_error1) {
    strcpy(expr, "d");
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_SYNTAX_ERROR,
        "expr \"%s\" returns DE_SYNTAX_ERROR", expr);
}
END_TEST

START_TEST(syntax_error2) {
    strcpy(expr, "1d");
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_SYNTAX_ERROR,
        "expr \"%s\" returns DE_SYNTAX_ERROR", expr);
}
END_TEST

START_TEST(syntax_error3) {
    strcpy(expr, "1d<");
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_SYNTAX_ERROR,
        "expr \"%s\" returns DE_SYNTAX_ERROR", expr);
}
END_TEST

START_TEST(syntax_error4) {
    strcpy(expr, "1d1+<");
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_SYNTAX_ERROR,
        "expr \"%s\" returns DE_SYNTAX_ERROR", expr);
}
END_TEST

START_TEST(syntax_error5) {
    strcpy(expr, "-d+1");
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_SYNTAX_ERROR,
        "expr \"%s\" returns DE_SYNTAX_ERROR", expr);
}
END_TEST

START_TEST(overflow) {
    const char OVERFLOW_INT_LEAST64_T[] = "9223372036854775808";
    strcpy(expr, OVERFLOW_INT_LEAST64_T);
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_OVERFLOW,
        "expr \"%s\" returns DE_OVERFLOW", expr);
}
END_TEST

START_TEST(bug_overflow1) {
    /* Number of rolls is relevant, expression was generated randomly by a
     * fuzzer. Expression was originally 999999919999910d100>3+2, but to make
     * this test probably work also on a 64-bit processor, we use larger number
     * of rolls. */
    strcpy(expr, "99999991999991099999d100>3+2");
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_OVERFLOW, "expr \"%s\" overflows", expr);
}
END_TEST

Suite*
suite_diceexpr_invalid() {
    Suite *suite = suite_create("diceexpr_invalid");
    TCase *tcase = tcase_create("Core");
    suite_add_tcase(suite, tcase);
    tcase_add_checked_fixture(tcase, setup, teardown);

    tcase_add_test(tcase, bug_68020b7);
    tcase_add_test(tcase, invalid_character);
    tcase_add_test(tcase, zero_nrolls);
    tcase_add_test(tcase, zero_dice);
    tcase_add_test(tcase, ignore_small);
    tcase_add_test(tcase, ignore_large);
    tcase_add_test(tcase, syntax_error1);
    tcase_add_test(tcase, syntax_error2);
    tcase_add_test(tcase, syntax_error3);
    tcase_add_test(tcase, syntax_error4);
    tcase_add_test(tcase, syntax_error5);
    tcase_add_test(tcase, overflow);
    tcase_add_test(tcase, bug_overflow1);

    return suite;
}
