#include "str.h"
#include "test.h"
#include <string.h>

static str *s;

static void
setup() {
    s = str_new(NULL);
}

static void
teardown() {
    str_free(s);
}

START_TEST(erase_empty) {
    str_erase(s);

    ck_assert_msg(strcmp(s->str, "") == 0, "is empty string");
    ck_assert_msg(s->len == 0, "Length of an erased empty string is 0");
}
END_TEST


START_TEST(erase1) {
    str_append_chars(s, "a");

    str_erase(s);

    ck_assert_msg(strcmp(s->str, "") == 0, "is empty string");
    ck_assert_msg(s->len == 0, "Length of an erased empty string is 0");
}
END_TEST

START_TEST(erase2) {
    str_append_chars(s, "1234567890");

    str_erase(s);

    ck_assert_msg(strcmp(s->str, "") == 0, "is empty string");
    ck_assert_msg(s->len == 0, "Length of an erased empty string is 0");
}
END_TEST

Suite*
suite_str_erase() {
    Suite *suite = suite_create("str_erase");
    TCase *tcase = tcase_create("Core");
    suite_add_tcase(suite, tcase);
    tcase_add_checked_fixture(tcase, setup, teardown);

    tcase_add_test(tcase, erase_empty);
    tcase_add_test(tcase, erase1);
    tcase_add_test(tcase, erase2);

    return suite;
}
