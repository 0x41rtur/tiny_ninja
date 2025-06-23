#include "test_executor.h"

#include "clip.h"
#include "common.h"
#include "executer.h"
#include "unity.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#define TMP_FILE_NAME "tmp_test_file.txt"
#define TMP_DIR_NAME  "tmp_test_dir"

void
test_execute_null_tnt(void)
{
        int err = 0;
        TEST_ASSERT_EQUAL_INT(-1, execute(&err, NULL));
        TEST_ASSERT_EQUAL_INT(EXECUTOR_ERR_BAD_ARG, err);
}

void
test_execute_bad_directory(void)
{
        struct command cmd = {
            .ext = "txt",
            .dir = "/\x01\xFF" // некорректный путь для make_dir_recursive
        };
        const struct tnt t   = {.name = TMP_FILE_NAME, .cmd = &cmd};
        int              err = 0;
        TEST_ASSERT_EQUAL_INT(-1, execute(&err, &t));
        TEST_ASSERT_EQUAL_INT(EXECUTOR_ERR_BAD_ARG, err);
}

void
test_execute_concat_failure(void)
{
        struct command cmd = {
            .ext = "txt",
            .dir = NULL // concat упадёт, если не проверяет на NULL
        };
        const struct tnt t   = {.name = TMP_FILE_NAME, .cmd = &cmd};
        int              err = 0;
        TEST_ASSERT_EQUAL_INT(-1, execute(&err, &t));
        TEST_ASSERT_EQUAL_INT(EXECUTOR_ERR_CREATE_PATH, err);
}

void
test_execute_file_exists(void)
{
        FILE *f = fopen(TMP_FILE_NAME, "w");
        TEST_ASSERT_NOT_NULL(f);
        fprintf(f, "hello");
        fclose(f);
        mkdir(TMP_DIR_NAME, 0755);
        char *target = concat(TMP_DIR_NAME, "/", TMP_FILE_NAME, NULL);
        TEST_ASSERT_NOT_NULL(target);
        FILE *existing = fopen(target, "w");
        TEST_ASSERT_NOT_NULL(existing);
        fprintf(existing, "conflict");
        fclose(existing);
        struct command   cmd = {.ext = "txt", .dir = TMP_DIR_NAME};
        const struct tnt t   = {.name = TMP_FILE_NAME, .cmd = &cmd};
        int              err = 0;
        TEST_ASSERT_EQUAL_INT(-1, execute(&err, &t));
        TEST_ASSERT_EQUAL_INT(EXECUTOR_ERR_FILE_EXISTS, err);
        remove(TMP_FILE_NAME);
        remove(target);
        rmdir(TMP_DIR_NAME);
        free(target);
}

void
test_execute_rename_failure(void)
{
        struct command   cmd = {.ext = "txt", .dir = TMP_DIR_NAME};
        const struct tnt t   = {.name = "nonexistent_input.txt", // файла нет
                                .cmd  = &cmd};
        mkdir(TMP_DIR_NAME, 0755);
        int err = 0;
        TEST_ASSERT_EQUAL_INT(-1, execute(&err, &t));
        TEST_ASSERT_EQUAL_INT(EXECUTOR_ERR_MV, err);
        rmdir(TMP_DIR_NAME);
}

void
test_execute_success(void)
{
        FILE *f = fopen(TMP_FILE_NAME, "w");
        TEST_ASSERT_NOT_NULL(f);
        fprintf(f, "success!");
        fclose(f);
        struct command   cmd = {.ext = "txt", .dir = TMP_DIR_NAME};
        const struct tnt t   = {.name = TMP_FILE_NAME, .cmd = &cmd};
        mkdir(TMP_DIR_NAME, 0755);
        int err = 0;
        TEST_ASSERT_EQUAL_INT(0, execute(&err, &t));
        TEST_ASSERT_EQUAL_INT(0, err);
        char *dst = concat(TMP_DIR_NAME, "/", TMP_FILE_NAME, NULL);
        TEST_ASSERT_NOT_NULL(dst);
        TEST_ASSERT_EQUAL(0, access(dst, F_OK));
        remove(dst);
        free(dst);
        rmdir(TMP_DIR_NAME);
}
