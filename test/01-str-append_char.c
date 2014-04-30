#include "str.h"
#include "test.h"
#include <string.h>

static str *s;

static
void teardown() {
    str_free(s);
}

START_TEST(append_to_NULL_string) {
    s = str_new(NULL);

    const char *t = "12345678901234567890";
    for (size_t i = 0; t[i]; i++) {
        str_append_char(s, t[i]);

        ck_assert_msg(s->str[i] == t[i],
            "'%c' is appended to the string", t[i]);
        ck_assert_msg(s->len == i + 1, "t's len member is %d", i + 1);
    }
    ck_assert_msg(strcmp(s->str, t) == 0, "%s == %s", s->str, t);
}
END_TEST

START_TEST(append_to_empty_string) {
    s = str_new("");

    str_append_char(s, 'a');

    ck_assert_msg(strcmp(s->str, "a") == 0, "%s == %s", s->str, "a");
    ck_assert_msg(s->len == 1, "str's len member is now %d", 1);
}
END_TEST

START_TEST(append_to_non_empty_string) {
    s = str_new("pera");

    str_append_char(s, 'a');

    ck_assert_msg(strcmp(s->str, "peraa") == 0, "%s == %s", s->str, "peraa");
    ck_assert_msg(s->len == 5, "str's len member is now %d", 5);
}
END_TEST

Suite*
suite_str_append_char() {
    Suite *suite = suite_create("str_append_char");
    TCase *tcase = tcase_create("Core");
    suite_add_tcase(suite, tcase);
    tcase_add_checked_fixture(tcase, NULL, teardown);

    tcase_add_test(tcase, append_to_NULL_string);
    tcase_add_test(tcase, append_to_empty_string);
    tcase_add_test(tcase, append_to_non_empty_string);

    return suite;
}
