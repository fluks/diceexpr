#include "test.h"
#include "diceexpr.h"
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <regex.h>

static char *rolled_expr;
static int_least64_t value;
static enum parse_error error;
#define EXPR_SIZE 100
static char expr[EXPR_SIZE];
static regex_t regexp;

static void
setup() {
    rolled_expr = NULL;
    value = 0;
    error = 0;
    memset(&regexp, 0, sizeof(regexp));
}

static void
teardown() {
    free(rolled_expr);
    regfree(&regexp);
}

static int
match(regex_t *re, const char *pattern, const char *s) {
    if (regcomp(re, pattern, REG_EXTENDED | REG_NOSUB) == 1) {
        ck_abort_msg("failed to compile regex");
    }
    return regexec(re, s, 0, NULL, 0);
}

START_TEST(integers1) {
    strcpy(expr, "1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 1);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(unary_minus) {
    strcpy(expr, "-1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, -1);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(unary_plus) {
    strcpy(expr, "+1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 1);
    ck_assert_str_eq("+1", rolled_expr);
}
END_TEST

START_TEST(zero) {
    strcpy(expr, "0");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 0);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(max) {
    sprintf(expr, "%" PRIdLEAST64, INT_LEAST64_MAX);
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, INT_LEAST64_MAX);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(min) {
    // Can't be INT_LEAST64_MIN, because '-' would be read first, then the
    // number, * which would be INT_LEAST64_MAX + 1.
    sprintf(expr, "%" PRIdLEAST64, INT_LEAST64_MIN + 1);
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, INT_LEAST64_MIN + 1);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(addition1) {
    strcpy(expr, "1+1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 2);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(addition2) {
    strcpy(expr, "-1+1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 0);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(addition3) {
    strcpy(expr, "-1+99+102");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 200);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(subtract1) {
    strcpy(expr, "1-1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 0);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(subtract2) {
    strcpy(expr, "100-1000-1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, -901);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(multiple_plus_and_or_minus_sequence1) {
    strcpy(expr, "--1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 1);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(multiple_plus_and_or_minus_sequence2) {
    strcpy(expr, "-1--1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 0);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(multiple_plus_and_or_minus_sequence3) {
    strcpy(expr, "++1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 1);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(multiple_plus_and_or_minus_sequence4) {
    strcpy(expr, "1-+1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 0);
    ck_assert_str_eq(expr, rolled_expr);
}
END_TEST

START_TEST(dice1) {
    strcpy(expr, "d1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 1);
    ck_assert_str_eq("(1)", rolled_expr);
}
END_TEST

START_TEST(dice2) {
    strcpy(expr, "1d1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 1);
    ck_assert_str_eq("(1)", rolled_expr);
}
END_TEST

START_TEST(dice3) {
    strcpy(expr, "2d1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 2);
    ck_assert_str_eq("(1+1)", rolled_expr);
}
END_TEST

START_TEST(dice4) {
    strcpy(expr, "5d1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 5);
    ck_assert_str_eq("(1+1+1+1+1)", rolled_expr);
}
END_TEST

START_TEST(dice5) {
    strcpy(expr, "-3d1+4d1-1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 0);
    ck_assert_str_eq("-(1+1+1)+(1+1+1+1)-1", rolled_expr);
}
END_TEST

START_TEST(dice6) {
    strcpy(expr, "2d1<-5d1<2>2+1");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_int_eq(value, 1);
    ck_assert_str_eq("(1)-(1)+1", rolled_expr);
}
END_TEST

START_TEST(dice7) {
    strcpy(expr, "d6");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(value >= 1 && value <= 6, "1 <= value <= 6");
    int matched = match(&regexp, "^\\([1-6]\\)$", rolled_expr);
    ck_assert_uint_eq(matched, 0);
}
END_TEST

START_TEST(dice8) {
    strcpy(expr, "3d6");
    de_parse(expr, &value, &rolled_expr);

    ck_assert_msg(value >= 3 && value <= 18, "3 <= value <= 18");
    int matched = match(&regexp, "^\\(([1-6]\\+){2}[1-6]\\)$", rolled_expr);
    ck_assert_uint_eq(matched, 0);
}
END_TEST

Suite*
suite_diceexpr_valid() {
    Suite *suite = suite_create("diceexpr_valid");
    TCase *tcase = tcase_create("Core");
    suite_add_tcase(suite, tcase);
    tcase_add_checked_fixture(tcase, setup, teardown);

    tcase_add_test(tcase, integers1);
    tcase_add_test(tcase, unary_minus);
    tcase_add_test(tcase, unary_plus);
    tcase_add_test(tcase, zero);
    tcase_add_test(tcase, max);
    tcase_add_test(tcase, min);
    tcase_add_test(tcase, addition1);
    tcase_add_test(tcase, addition2);
    tcase_add_test(tcase, addition3);
    tcase_add_test(tcase, subtract1);
    tcase_add_test(tcase, subtract2);
    tcase_add_test(tcase, multiple_plus_and_or_minus_sequence1);
    tcase_add_test(tcase, multiple_plus_and_or_minus_sequence2);
    tcase_add_test(tcase, multiple_plus_and_or_minus_sequence3);
    tcase_add_test(tcase, multiple_plus_and_or_minus_sequence4);
    tcase_add_test(tcase, dice1);
    tcase_add_test(tcase, dice2);
    tcase_add_test(tcase, dice3);
    tcase_add_test(tcase, dice4);
    tcase_add_test(tcase, dice5);
    tcase_add_test(tcase, dice6);
    tcase_add_test(tcase, dice7);
    tcase_add_test(tcase, dice8);

    return suite;
}
