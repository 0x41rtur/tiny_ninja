#include "test_clip.h"
#include "test_opt_d.h"
#include "test_opt_e.h"
#include "test_opt_m.h"

#include "unity.h"

void
setUp(void)
{ /* инициализация, если нужна */
}
void
tearDown(void)
{ /* очистка, если нужна */
}

int
main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_opt_d_null);
        RUN_TEST(test_opt_d_empty_string);
        RUN_TEST(test_opt_d_double_slash_start);
        RUN_TEST(test_opt_d_double_slash_end);
        RUN_TEST(test_opt_d_double_slash_middle);
        RUN_TEST(test_opt_d_dot_segment);
        RUN_TEST(test_opt_d_only_spaces);
        RUN_TEST(test_opt_d_single_slash);
        RUN_TEST(test_opt_d_single_dot);
        RUN_TEST(test_opt_d_double_dot);
        RUN_TEST(test_opt_d_dot_slash);
        RUN_TEST(test_opt_d_too_long_string);
        RUN_TEST(test_opt_d_dir);
        RUN_TEST(test_opt_d_nested_dir);
        RUN_TEST(test_opt_d_root_dir);
        RUN_TEST(test_opt_d_nested_root_dir);
        RUN_TEST(test_opt_d_nested_root_slash);
        RUN_TEST(test_opt_d_deep_nested_dir);

        RUN_TEST(test_arge_dot_only);
        RUN_TEST(test_arge_dot_prefix);
        RUN_TEST(test_arge_path_instead_ext);
        RUN_TEST(test_arge_only_spaces);
        RUN_TEST(test_arge_dot_inside);
        RUN_TEST(test_arge_empty_string);
        RUN_TEST(test_arge_null);
        RUN_TEST(test_arge_contains_space);
        RUN_TEST(test_arge_single_letter);
        RUN_TEST(test_arge_valid_normal);
        RUN_TEST(test_arge_valid_longer);

        RUN_TEST(test_argm_null);
        RUN_TEST(test_argm_missing_mapping);
        RUN_TEST(test_argm_bad_dir_syntax);
        RUN_TEST(test_argm_bad_ext_syntax);
        RUN_TEST(test_argm_double_mapping_with_dot);
        RUN_TEST(test_argm_dir_with_dot_slash);
        RUN_TEST(test_argm_spaces_in_input);
        RUN_TEST(test_argm_unfinished_mapping);
        RUN_TEST(test_argm_valid_single);
        RUN_TEST(test_argm_valid_multiple);
        RUN_TEST(test_argm_valid_duplicate_keys);

        RUN_TEST(test_clip_invalid_dir_empty);
        RUN_TEST(test_clip_invalid_empty_args);
        RUN_TEST(test_clip_invalid_ext_empty);
        RUN_TEST(test_clip_invalid_map_arg);
        RUN_TEST(test_clip_valid_single_pair);
        RUN_TEST(test_clip_valid_mapping);
        RUN_TEST(test_clip_valid_trailing_slash);
        RUN_TEST(test_copy_command_null_cmd);
        RUN_TEST(test_copy_command_null_ext);
        RUN_TEST(test_copy_command_null_dir);
        RUN_TEST(test_copy_command_valid);

        return UNITY_END();
}
