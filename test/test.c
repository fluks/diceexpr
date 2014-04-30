#include "test.h"
#include <stdlib.h>
#include <time.h>

int
main() {
    SRunner *sr = srunner_create(suite_str_new());
    srunner_add_suite(sr, suite_str_append_char());
    srunner_add_suite(sr, suite_str_append_chars());
    srunner_add_suite(sr, suite_str_erase());
    srunner_add_suite(sr, suite_str_append_format());
    srunner_add_suite(sr, suite_str_copy_to_chars());

    // Must be called before de_parse().
    srand(time(NULL));

    srunner_add_suite(sr, suite_diceexpr_valid());
    srunner_add_suite(sr, suite_diceexpr_invalid());

    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);

    srunner_free(sr);

    exit(number_failed > 0 ? number_failed : EXIT_SUCCESS);
}
