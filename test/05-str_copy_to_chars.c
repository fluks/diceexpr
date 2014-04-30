#include "str.h"
#include "test.h"
#include <string.h>
#include <stdlib.h>

static str *s;
static char *copy;

static void
setup() {
    s = str_new(NULL);
    copy = NULL;
}

static void
teardown() {
    str_free(s);
    free(copy);
}

START_TEST(copy_empty) {
    str_copy_to_chars(s, &copy);

    ck_assert_msg(strcmp(copy, "") == 0, "%s == %s", copy, "");
}
END_TEST

START_TEST(copy1) {
    const char *t = "a";
    str_append_chars(s, t);
    str_copy_to_chars(s, &copy);

    ck_assert_msg(strcmp(copy, t) == 0, "%s == %s", copy, t);
}
END_TEST

START_TEST(copy2) {
    const char *t = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    str_append_chars(s, t);
    str_copy_to_chars(s, &copy);

    ck_assert_msg(strcmp(copy, t) == 0, "%s == %s", copy, t);
}
END_TEST

Suite*
suite_str_copy_to_chars() {
    Suite *suite = suite_create("str_copy_to_chars");
    TCase *tcase = tcase_create("Core");
    suite_add_tcase(suite, tcase);
    tcase_add_checked_fixture(tcase, setup, teardown);

    tcase_add_test(tcase, copy_empty);
    tcase_add_test(tcase, copy1);
    tcase_add_test(tcase, copy2);

    return suite;
}
