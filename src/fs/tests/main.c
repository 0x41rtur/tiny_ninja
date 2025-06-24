#include "test_fs.h"
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
        RUN_TEST(test_is_target_valid_txt_file);
        RUN_TEST(test_is_target_invalid_ext);
        RUN_TEST(test_is_target_no_extension);
        RUN_TEST(test_is_target_nonexistent_file);
        RUN_TEST(test_mk_dir_create_new);
        RUN_TEST(test_mk_dir_existing);
        RUN_TEST(test_make_dir_recursive_create);
        RUN_TEST(test_make_dir_recursive_existing);
        RUN_TEST(test_make_dir_recursive_invalid);
        UNITY_END();
        return 0;
}
