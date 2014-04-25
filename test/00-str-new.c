#include "DS_dllist.h"
#include "test.h"
#include <string.h>

static DS_dllist *list;

void
setup() {
    list = DS_dllist_new();
}

void
teardown() {
    DS_dllist_free(list);
}

START_TEST(test_DS_dllist_new) {
    ck_assert_msg(list != NULL, "DS_dllist_new returns not NULL");
    ck_assert_msg(list->head == NULL, "list's head is NULL");
    ck_assert_msg(list->tail == NULL, "list's tail is NULL");
}
END_TEST

START_TEST(test_DS_dllist_append1) {
    char s1[] = "a";
    DS_dllist_node *node1 = DS_dllist_append(list, s1, NULL);

    ck_assert_msg(node1 != NULL, "append succeeded");
    ck_assert_msg(strcmp((char*) node1->data, s1) == 0,
        "appeded node's data is \"%s\"", s1);
    ck_assert_msg(!node1->next && !node1->prev,
        "appended node's next and prev are NULLs");
    ck_assert_msg(!node1->data_is_a_copy, "appended node's data isn't copied");
    ck_assert_msg(list->head == node1, "list's head is appended node");
    ck_assert_msg(list->tail == node1, "list's tail is appended node");

    char s2[] = "abc";
    DS_dllist_node *node2 = DS_dllist_append(list, s2, NULL);

    ck_assert_msg(node2 != NULL, "appended a \"%s\"", s2);
    ck_assert_msg(node2->prev == node1, "prev is same as head");
    ck_assert_msg(node1->next == node2, "node1's next is node2");
    ck_assert_msg(list->head == node1, "list's head is node1");
    ck_assert_msg(list->tail == node2, "list's tail is node2");

    DS_dllist_free_all(list);
    list = NULL;
}
END_TEST

Suite*
DS_dllist_suite() {
    Suite *suite = suite_create("DS_dllist");
    TCase *tcase = tcase_create("Core");
    tcase_add_checked_fixture(tcase, setup, teardown);
    suite_add_tcase(suite, tcase);

    tcase_add_test(tcase, test_DS_dllist_new);
    tcase_add_test(tcase, test_DS_dllist_append1);

    return suite;
}
