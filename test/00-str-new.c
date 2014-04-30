#include "test.h"
#include "str.h"
#include <string.h>

static str *s;
static char t[100];

START_TEST(null_param) {
    s = str_new(NULL);

    ck_assert_str_eq(s->str, "");
    ck_assert_uint_eq(s->len, 0);

    str_free(s);
}
END_TEST

START_TEST(empty_string_param) {
    strcpy(t, "");
    s = str_new(t);

    ck_assert_str_eq(s->str, t);
    ck_assert_uint_eq(s->len, strlen(t));

    str_free(s);
}
END_TEST

START_TEST(params) {
    strcpy(t, "12345678901234567890");

    for (size_t i = 0; i < strlen(t); i++) {
        char temp[100] = { 0 };
        strncpy(temp, t, i);
        s = str_new(temp);

        ck_assert_str_eq(s->str, temp);
        ck_assert_uint_eq(s->len, strlen(temp));

        str_free(s);
    }
}
END_TEST

Suite*
suite_str_new() {
    Suite *suite = suite_create("str_new");
    TCase *tcase = tcase_create("Core");
    suite_add_tcase(suite, tcase);

    tcase_add_test(tcase, null_param);
    tcase_add_test(tcase, empty_string_param);
    tcase_add_test(tcase, params);

    return suite;
}
