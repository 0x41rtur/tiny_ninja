#include "test_fs.h"

#include "unity.h"
#include "fs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define TMP_FILE       "tmp_fs_file.txt"
#define TMP_FILE_EXT   "txt"
#define TMP_DIR_NAME   "tmp_fs_dir"
#define TMP_NESTED_DIR "tmp_fs_dir/a/b"

void
test_is_target_valid_txt_file(void)
{
        FILE *f = fopen(TMP_FILE, "w");
        TEST_ASSERT_NOT_NULL(f);
        fprintf(f, "test\n");
        fclose(f);

        int r = is_target(TMP_FILE, TMP_FILE_EXT);
        TEST_ASSERT_EQUAL_INT(0, r);

        remove(TMP_FILE);
}

void
test_is_target_invalid_ext(void)
{
        FILE *f = fopen(TMP_FILE, "w");
        TEST_ASSERT_NOT_NULL(f);
        fprintf(f, "test\n");
        fclose(f);

        int r = is_target(TMP_FILE, "md");
        TEST_ASSERT_EQUAL_INT(-1, r);

        remove(TMP_FILE);
}

void
test_is_target_no_extension(void)
{
        FILE *f = fopen("file_no_ext", "w");
        TEST_ASSERT_NOT_NULL(f);
        fclose(f);

        TEST_ASSERT_EQUAL_INT(-1, is_target("file_no_ext", "txt"));

        remove("file_no_ext");
}

void
test_is_target_nonexistent_file(void)
{
        TEST_ASSERT_EQUAL_INT(-1,
                              is_target("definitely_not_exists.txt", "txt"));
}

void
test_mk_dir_create_new(void)
{
        TEST_ASSERT_EQUAL(0, mk_dir(TMP_DIR_NAME));
        rmdir(TMP_DIR_NAME);
}

void
test_mk_dir_existing(void)
{
        mkdir(TMP_DIR_NAME, 0755);
        TEST_ASSERT_EQUAL(1, mk_dir(TMP_DIR_NAME));
        rmdir(TMP_DIR_NAME);
}

void
test_make_dir_recursive_create(void)
{
        TEST_ASSERT_EQUAL(0, make_dir_recursive(TMP_NESTED_DIR));

        rmdir("tmp_fs_dir/a/b");
        rmdir("tmp_fs_dir/a");
        rmdir("tmp_fs_dir");
}

void
test_make_dir_recursive_existing(void)
{
        mkdir("tmp_fs_dir", 0755);
        TEST_ASSERT_EQUAL(1, make_dir_recursive("tmp_fs_dir"));
        rmdir("tmp_fs_dir");
}

void
test_make_dir_recursive_invalid(void)
{
        // Попробуем создать директорию с именем, которое точно не сработает
        // Например, пустая строка
        TEST_ASSERT_EQUAL(-1, make_dir_recursive(""));
}
