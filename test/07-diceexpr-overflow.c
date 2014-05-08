#include "test.h"
#include "diceexpr.h"
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

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

START_TEST(overflow1) {
    sprintf(expr, "%" PRIdLEAST64 "+1", INT_LEAST64_MAX);
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_OVERFLOW,
        "expr \"%s\" returns DE_OVERFLOW", expr);
}
END_TEST

START_TEST(overflow2) {
    sprintf(expr, "-%" PRIdLEAST64, INT_LEAST64_MIN);
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_OVERFLOW,
        "expr \"%s\" returns DE_OVERFLOW", expr);
}
END_TEST

START_TEST(overflow3) {
    sprintf(expr, "%" PRIdLEAST64 "-1", INT_LEAST64_MIN);
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_OVERFLOW,
        "expr \"%s\" returns DE_OVERFLOW", expr);
}
END_TEST

START_TEST(overflow4) {
    // Just very likely to overflow.
    sprintf(expr, "100d%" PRIdLEAST64, INT_LEAST64_MAX);
    error = de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(error == DE_OVERFLOW,
        "expr \"%s\" returns DE_OVERFLOW", expr);
}
END_TEST

Suite*
suite_diceexpr_overflow() {
    Suite *suite = suite_create("diceexpr_overflow");
    TCase *tcase = tcase_create("Core");
    suite_add_tcase(suite, tcase);
    tcase_add_checked_fixture(tcase, setup, teardown);

    tcase_add_test(tcase, overflow1);
    tcase_add_test(tcase, overflow2);
    tcase_add_test(tcase, overflow3);
    tcase_add_test(tcase, overflow4);

    return suite;
}
