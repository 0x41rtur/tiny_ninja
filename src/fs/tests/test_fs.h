#ifndef TEST_FS_H
#define TEST_FS_H

void test_is_target_valid_txt_file(void);
void test_is_target_invalid_ext(void);
void test_is_target_no_extension(void);
void test_is_target_nonexistent_file(void);
void test_mk_dir_create_new(void);
void test_mk_dir_existing(void);
void test_make_dir_recursive_create(void);
void test_make_dir_recursive_existing(void);
void test_make_dir_recursive_invalid(void);

#endif // TEST_FS_H
