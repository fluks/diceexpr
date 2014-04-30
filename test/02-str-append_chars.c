#include "str.h"
#include "test.h"
#include <string.h>

static str *s;

static
void teardown() {
    str_free(s);
}

START_TEST(append_chars_to_NULL_string) {
    s = str_new(NULL);

    const char *t = "a";
    str_append_chars(s, t);

    ck_assert_msg(strcmp(s->str, t) == 0, "%s == %s", s->str, t);
    ck_assert_msg(s->len == 1, "str's len member is %d", 1);

    str_append_chars(s, t);

    ck_assert_msg(strcmp(s->str, "aa") == 0, "%s == %s", s->str, "aa");
    ck_assert_msg(s->len == 2, "str's len member is %d", 2);
}
END_TEST

START_TEST(append_chars_to_empty_string) {
    s = str_new("");

    const char *t = "a";
    str_append_chars(s, t);

    ck_assert_msg(strcmp(s->str, t) == 0, "%s == %s", s->str, t);
    ck_assert_msg(s->len == 1, "str's len member is %d", 1);
}
END_TEST

START_TEST(append_chars_to_non_empty_string) {
    s = str_new("a");

    const char *t = "a";
    str_append_chars(s, t);

    ck_assert_msg(strcmp(s->str, "aa") == 0, "%s == %s", s->str, "aa");
    ck_assert_msg(s->len == 2, "str's len member is %d", 2);
}
END_TEST

START_TEST(append_chars_long) {
    s = str_new(NULL);

    const char *t = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    str_append_chars(s, t);

    ck_assert_msg(strcmp(s->str, t) == 0, "%s == %s", s->str, t);
    ck_assert_msg(s->len == strlen(t), "str's len member is %d", strlen(t));
}
END_TEST

Suite*
suite_str_append_chars() {
    Suite *suite = suite_create("str_append_chars");
    TCase *tcase = tcase_create("Core");
    suite_add_tcase(suite, tcase);
    tcase_add_checked_fixture(tcase, NULL, teardown);

    tcase_add_test(tcase, append_chars_to_NULL_string);
    tcase_add_test(tcase, append_chars_to_empty_string);
    tcase_add_test(tcase, append_chars_to_non_empty_string);
    tcase_add_test(tcase, append_chars_long);

    return suite;
}
