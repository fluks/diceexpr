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

START_TEST(append_format1) {
    str_append_format(s, "%s", "a");

    ck_assert_msg(strcmp(s->str, "a") == 0, "%s == %s", s->str, "a");
}
END_TEST

START_TEST(append_format2) {
    str_append_format(s, "%.3s", "aaaa");

    ck_assert_msg(strcmp(s->str, "aaa") == 0, "%s == %s", s->str, "aaa");
}
END_TEST

START_TEST(append_format3) {
    str_append_format(s, "%d", 123);

    ck_assert_msg(strcmp(s->str, "123") == 0, "%s == %s", s->str, 123);
}
END_TEST

Suite*
suite_str_append_format() {
    Suite *suite = suite_create("str_append_format");
    TCase *tcase = tcase_create("Core");
    suite_add_tcase(suite, tcase);
    tcase_add_checked_fixture(tcase, setup, teardown);

    tcase_add_test(tcase, append_format1);
    tcase_add_test(tcase, append_format2);
    tcase_add_test(tcase, append_format3);

    return suite;
}
