#ifndef TEST_COMMON_H
#define TEST_COMMON_H

void
test_strcopy_valid(void);
void
test_strcopy_invalid(void);
void
test_strcopy_null(void);
void
test_strncopy_valid(void);
void
test_strncopy_invalid(void);
void
test_strncopy_null(void);
void
test_strncopy_zero_length(void);
void
test_strncopy_length_greater_than_string(void);
void
test_split_valid(void);
void
test_split_invalid(void);
void
test_split_null_input(void);
void
test_split_empty_string(void);
void
test_find_ext_suffix_valid(void);
void
test_find_ext_suffix_invalid(void);
void
test_is_regular_file_valid(void);
void
test_is_regular_file_missing(void);
void
test_concat_valid(void);
void
test_concat_null(void);
void
test_concat_with_null_middle(void);
void
test_strtokarr_valid(void);
void
test_strtokarr_null(void);
void
test_strtokarr_empty_string(void);
void
test_strtokarr_no_delimiter(void);

#endif //TEST_COMMON_H
