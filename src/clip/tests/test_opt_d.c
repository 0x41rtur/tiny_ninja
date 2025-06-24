#include "test_opt_d.h"

#include "internal/opt_d.h"
#include "unity.h"

#include <stdlib.h>
#include <linux/limits.h>

void
test_opt_d_null(void)
{
        TEST_ASSERT_EQUAL_INT(-1, argd(NULL));
}
void
test_opt_d_empty_string(void)
{
        TEST_ASSERT_EQUAL_INT(-1, argd(""));
}
void
test_opt_d_double_slash_start(void)
{
        TEST_ASSERT_EQUAL_INT(-1, argd("//dir"));
}
void
test_opt_d_double_slash_end(void)
{
        TEST_ASSERT_EQUAL_INT(-1, argd("dir//"));
}
void
test_opt_d_double_slash_middle(void)
{
        TEST_ASSERT_EQUAL_INT(-1, argd("dir//dir"));
}
void
test_opt_d_dot_segment(void)
{
        TEST_ASSERT_EQUAL_INT(-1, argd("dir/./dir"));
}
void
test_opt_d_only_spaces(void)
{
        TEST_ASSERT_EQUAL_INT(-1, argd("   "));
}
void
test_opt_d_single_slash(void)
{
        TEST_ASSERT_EQUAL_INT(-1, argd("/"));
}
void
test_opt_d_single_dot(void)
{
        TEST_ASSERT_EQUAL_INT(-1, argd("."));
}
void
test_opt_d_double_dot(void)
{
        TEST_ASSERT_EQUAL_INT(-1, argd(".."));
}
void
test_opt_d_dot_slash(void)
{
        TEST_ASSERT_EQUAL_INT(-1, argd("./"));
}

void
test_opt_d_too_long_string(void)
{
        char *max = calloc(PATH_MAX, sizeof(char));
        TEST_ASSERT_NOT_NULL(max);
        for (int i = 0; i < PATH_MAX - 1; i++)
        {
                max[i] = 'a';
        }
        max[PATH_MAX - 1] = '\0';
        TEST_ASSERT_EQUAL_INT(-1, argd(max));
        free(max);
}

void
test_opt_d_dir(void)
{
        TEST_ASSERT_EQUAL_INT(0, argd("dir"));
}
void
test_opt_d_nested_dir(void)
{
        TEST_ASSERT_EQUAL_INT(0, argd("dir/dir"));
}
void
test_opt_d_root_dir(void)
{
        TEST_ASSERT_EQUAL_INT(0, argd("/dir"));
}
void
test_opt_d_nested_root_dir(void)
{
        TEST_ASSERT_EQUAL_INT(0, argd("/dir/dir"));
}
void
test_opt_d_nested_root_slash(void)
{
        TEST_ASSERT_EQUAL_INT(0, argd("/dir/dir/"));
}
void
test_opt_d_deep_nested_dir(void)
{
        TEST_ASSERT_EQUAL_INT(0, argd("dir/dir/dir/"));
}
