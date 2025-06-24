#include "test_common.h"

#include "unity.h"

void
setUp()
{ /* инициализация, если нужна */
}
void
tearDown()
{ /* очистка, если нужна */
}

int
main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_strcopy_valid);
        RUN_TEST(test_strcopy_invalid);
        RUN_TEST(test_strcopy_null);
        RUN_TEST(test_strncopy_valid);
        RUN_TEST(test_strncopy_invalid);
        RUN_TEST(test_strncopy_null);
        RUN_TEST(test_strncopy_zero_length);
        RUN_TEST(test_strncopy_length_greater_than_string);
        RUN_TEST(test_split_valid);
        RUN_TEST(test_split_invalid);
        RUN_TEST(test_split_null_input);
        RUN_TEST(test_split_empty_string);
        RUN_TEST(test_find_ext_suffix_valid);
        RUN_TEST(test_find_ext_suffix_invalid);
        RUN_TEST(test_is_regular_file_valid);
        RUN_TEST(test_is_regular_file_missing);
        RUN_TEST(test_concat_valid);
        RUN_TEST(test_concat_null);
        RUN_TEST(test_concat_with_null_middle);
        RUN_TEST(test_strtokarr_valid);
        RUN_TEST(test_strtokarr_null);
        RUN_TEST(test_strtokarr_empty_string);
        RUN_TEST(test_strtokarr_no_delimiter);
        UNITY_END();
        return 0;
}
