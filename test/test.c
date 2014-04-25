#include "test.h"
#include <stdlib.h>

int
main() {
    SRunner *sr = srunner_create(DS_dllist_suite());
    srunner_run_all(sr, CK_VERBOSE);
    int number_failed = srunner_ntests_failed(sr);

    srunner_free(sr);

    exit(number_failed > 0 ? number_failed : EXIT_SUCCESS);
}
