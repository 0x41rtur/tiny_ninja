#include "test_opt_m.h"
#include "clip.h"
#include "internal/opt_m.h"
#include "unity.h"

void
test_argm_null(void)
{
        int                    error = ARGM_OK;
        const struct command **cmd   = argm(&error, NULL);
        TEST_ASSERT_NULL(cmd);
        TEST_ASSERT_EQUAL_INT(ARGM_NULL_ARG, error);
}

void
test_argm_missing_mapping(void)
{
        int                    error = ARGM_OK;
        const struct command **cmd   = argm(&error, ".txt");
        TEST_ASSERT_NULL(cmd);
        TEST_ASSERT_EQUAL_INT(ARGM_INVALID_ARG, error);
}

void
test_argm_bad_dir_syntax(void)
{
        int                    error = ARGM_OK;
        const struct command **cmd   = argm(&error, "txt=//txt");
        TEST_ASSERT_NULL(cmd);
        TEST_ASSERT_EQUAL_INT(ARGM_INVALID_ARG, error);
}

void
test_argm_bad_ext_syntax(void)
{
        int                    error = ARGM_OK;
        const struct command **cmd   = argm(&error, ".txt=txt");
        TEST_ASSERT_NULL(cmd);
        TEST_ASSERT_EQUAL_INT(ARGM_INVALID_ARG, error);
}

void
test_argm_double_mapping_with_dot(void)
{
        int                    error = ARGM_OK;
        const struct command **cmd   = argm(&error, "txt=txt;.txt=txt");
        TEST_ASSERT_NULL(cmd);
        TEST_ASSERT_EQUAL_INT(ARGM_INVALID_ARG, error);
}

void
test_argm_dir_with_dot_slash(void)
{
        int                    error = ARGM_OK;
        const struct command **cmd   = argm(&error, "txt=./txt");
        TEST_ASSERT_NULL(cmd);
        TEST_ASSERT_EQUAL_INT(ARGM_INVALID_ARG, error);
}

void
test_argm_spaces_in_input(void)
{
        int                    error = ARGM_OK;
        const struct command **cmd   = argm(&error, "txt txt");
        TEST_ASSERT_NULL(cmd);
        TEST_ASSERT_EQUAL_INT(ARGM_INVALID_ARG, error);
}

void
test_argm_unfinished_mapping(void)
{
        int                    error = ARGM_OK;
        const struct command **cmd   = argm(&error, "txt=txt;txt");
        TEST_ASSERT_NULL(cmd);
        TEST_ASSERT_EQUAL_INT(ARGM_INVALID_ARG, error);
}

void
test_argm_valid_single(void)
{
        int                    error = ARGM_OK;
        const struct command **cmd   = argm(&error, "txt=txt");
        TEST_ASSERT_NOT_NULL(cmd);
        TEST_ASSERT_EQUAL_INT(ARGM_OK, error);
        TEST_ASSERT_EQUAL_STRING("txt", (*cmd)->ext);
        TEST_ASSERT_EQUAL_STRING("txt", (*cmd)->dir);
}

void
test_argm_valid_multiple(void)
{
        int                    error = ARGM_OK;
        const struct command **cmd   = argm(&error, "jpg=images;mp4=video");
        TEST_ASSERT_NOT_NULL(cmd);
        TEST_ASSERT_EQUAL_INT(ARGM_OK, error);
        TEST_ASSERT_EQUAL_STRING("jpg", cmd[0]->ext);
        TEST_ASSERT_EQUAL_STRING("images", cmd[0]->dir);
        TEST_ASSERT_EQUAL_STRING("mp4", cmd[1]->ext);
        TEST_ASSERT_EQUAL_STRING("video", cmd[1]->dir);
}

void
test_argm_valid_duplicate_keys(void)
{
        int                    error = ARGM_OK;
        const struct command **cmd   = argm(&error, "txt=dir/dir;txt=text");
        TEST_ASSERT_NOT_NULL(cmd);
        TEST_ASSERT_EQUAL_INT(ARGM_OK, error);
        TEST_ASSERT_EQUAL_STRING("txt", cmd[0]->ext);
        TEST_ASSERT_EQUAL_STRING("dir/dir", cmd[0]->dir);
        TEST_ASSERT_EQUAL_STRING("txt", cmd[1]->ext);
        TEST_ASSERT_EQUAL_STRING("text", cmd[1]->dir);
}
