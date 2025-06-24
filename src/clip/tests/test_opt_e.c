#include "test_opt_e.h"
#include "internal/opt_e.h"
#include "unity.h"

void
test_arge_dot_only(void)
{
        TEST_ASSERT_EQUAL_INT(-1, arge("."));
}
void
test_arge_dot_prefix(void)
{
        TEST_ASSERT_EQUAL_INT(-1, arge(".txt"));
}
void
test_arge_path_instead_ext(void)
{
        TEST_ASSERT_EQUAL_INT(-1, arge("./a.out"));
}
void
test_arge_only_spaces(void)
{
        TEST_ASSERT_EQUAL_INT(-1, arge("   "));
}
void
test_arge_dot_inside(void)
{
        TEST_ASSERT_EQUAL_INT(-1, arge("txt.txt"));
}
void
test_arge_empty_string(void)
{
        TEST_ASSERT_EQUAL_INT(-1, arge(""));
}
void
test_arge_null(void)
{
        TEST_ASSERT_EQUAL_INT(-1, arge(NULL));
}
void
test_arge_contains_space(void)
{
        TEST_ASSERT_EQUAL_INT(-1, arge("he lo"));
}

void
test_arge_single_letter(void)
{
        TEST_ASSERT_EQUAL_INT(0, arge("t"));
}
void
test_arge_valid_normal(void)
{
        TEST_ASSERT_EQUAL_INT(0, arge("txt"));
}
void
test_arge_valid_longer(void)
{
        TEST_ASSERT_EQUAL_INT(0, arge("txteee"));
}
