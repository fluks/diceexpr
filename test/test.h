#ifndef TEST_H
    #define TEST_H
#include <check.h>

Suite*
suite_str_new();

Suite*
suite_str_append_char();

Suite*
suite_str_erase();

Suite*
suite_str_append_chars();

Suite*
suite_str_append_format();

Suite*
suite_str_copy_to_chars();

Suite*
suite_diceexpr_valid();

Suite*
suite_diceexpr_invalid();

Suite*
suite_diceexpr_overflow();

#endif // TEST_H
