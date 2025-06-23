#include "test_executor.h"

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
        RUN_TEST(test_execute_null_tnt);
        RUN_TEST(test_execute_bad_directory);
        RUN_TEST(test_execute_concat_failure);
        RUN_TEST(test_execute_file_exists);
        RUN_TEST(test_execute_rename_failure);
        RUN_TEST(test_execute_success);

        UNITY_END();
        return 0;
}
