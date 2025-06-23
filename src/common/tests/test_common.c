#include "test_common.h"

#include "common.h"
#include "unity.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Тест копирования строки - валидный случай
void test_strcopy_valid(void)
{
    const char *s = "hello";
    char *copy = strcopy(s);
    TEST_ASSERT_NOT_NULL(copy);
    TEST_ASSERT_EQUAL_STRING("hello", copy);
    free(copy);
}

// Тест копирования строки - пустая строка
void test_strcopy_invalid(void)
{
    const char *s = "";
    char *copy = strcopy(s);
    TEST_ASSERT_NOT_NULL(copy);
    TEST_ASSERT_EQUAL_STRING("", copy);
    free(copy);
}

// Тест копирования строки - NULL
void test_strcopy_null(void)
{
    TEST_ASSERT_NULL(strcopy(NULL));
}

// Тест strncpy - валидный случай
void test_strncopy_valid(void)
{
    const char *s = "hello world";
    char *copy = strncopy(s, 5);
    TEST_ASSERT_NOT_NULL(copy);
    TEST_ASSERT_EQUAL_STRING("hello", copy);
    free(copy);
}

// Тест strncpy - пустая строка и длина 0
void test_strncopy_invalid(void)
{
    const char *s = "";
    char *copy = strncopy(s, 0);
    TEST_ASSERT_NOT_NULL(copy);
    TEST_ASSERT_EQUAL_STRING("", copy);
    free(copy);
}

// Тест strncpy - NULL
void test_strncopy_null(void)
{
    TEST_ASSERT_NULL(strncopy(NULL, 5));
}

// Тест strncpy - длина больше длины строки
void test_strncopy_zero_length(void)
{
    const char *s = "hello";
    char *copy = strncopy(s, 0);
    TEST_ASSERT_NOT_NULL(copy);
    TEST_ASSERT_EQUAL_STRING("", copy);
    free(copy);
}

// Тест strncpy - длина больше длины строки
void test_strncopy_length_greater_than_string(void)
{
    const char *s = "hello";
    char *copy = strncopy(s, 100);
    TEST_ASSERT_NOT_NULL(copy);
    TEST_ASSERT_EQUAL_STRING("hello", copy);
    free(copy);
}

// Тест split - валидный кейс "key=value"
void test_split_valid(void)
{
    const char *s = "key=value";
    char *key = NULL, *val = NULL;
    TEST_ASSERT_EQUAL_INT(0, split(s, &key, &val, '='));
    TEST_ASSERT_EQUAL_STRING("key", key);
    TEST_ASSERT_EQUAL_STRING("value", val);
    free(key);
    free(val);
}

// Тест split - строка без разделителя
void test_split_invalid(void)
{
    const char *s = "no_delim";
    char *a = NULL, *b = NULL;
    TEST_ASSERT_EQUAL_INT(-1, split(s, &a, &b, '='));
}

// Тест split - NULL на входе
void test_split_null_input(void)
{
    char *a = NULL, *b = NULL;
    TEST_ASSERT_EQUAL_INT(-1, split(NULL, &a, &b, '='));
}

// Тест split - пустая строка
void test_split_empty_string(void)
{
    char *a = NULL, *b = NULL;
    TEST_ASSERT_EQUAL_INT(-1, split("", &a, &b, '='));
}

// Тест find_ext_suffix - валидный кейс
void test_find_ext_suffix_valid(void)
{
    const char *f = "file.tar.gz";
    char *ext = find_ext_suffix(f);
    TEST_ASSERT_NOT_NULL(ext);
    TEST_ASSERT_EQUAL_STRING("gz", ext);
    free(ext);
}

// Тест find_ext_suffix - невалидные варианты
void test_find_ext_suffix_invalid(void)
{
    TEST_ASSERT_NULL(find_ext_suffix("file."));
    TEST_ASSERT_NULL(find_ext_suffix("file"));
    TEST_ASSERT_NULL(find_ext_suffix(NULL));
}

// Тест is_regular_file - файл существует
void test_is_regular_file_valid(void)
{
    FILE *f = fopen("testfile", "w");
    if (f)
    {
        fprintf(f, "test");
        fclose(f);
        TEST_ASSERT_EQUAL(0, is_regular_file("testfile"));
        remove("testfile");
    }
    else
    {
        TEST_IGNORE_MESSAGE("Cannot create test file");
    }
}

// Тест is_regular_file - файла нет
void test_is_regular_file_missing(void)
{
    TEST_ASSERT_EQUAL(-1, is_regular_file("not_existing_file"));
}

// Тест concat - несколько строк
void test_concat_valid(void)
{
    char *result = concat("Hello", " ", "world", "!", NULL);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("Hello world!", result);
    free(result);
}

// Тест concat - NULL вместо строки (функция должна обрабатывать или возвращать NULL)
void test_concat_null(void)
{
    TEST_ASSERT_NULL(concat(NULL));
}

// Тест concat - NULL в середине аргументов
void test_concat_with_null_middle(void)
{
    char *result = concat("Hello", NULL, "world", NULL);
    TEST_ASSERT_NOT_NULL(result);
    // Ожидается, что NULL игнорируется и строки объединяются без NULL
    TEST_ASSERT_EQUAL_STRING("Helloworld", result);
    free(result);
}

// Тест strtokarr - обычный кейс
void test_strtokarr_valid(void)
{
    char **arr = strtokarr("a;b;c", ';');
    TEST_ASSERT_NOT_NULL(arr);
    TEST_ASSERT_EQUAL_STRING("a", arr[0]);
    TEST_ASSERT_EQUAL_STRING("b", arr[1]);
    TEST_ASSERT_EQUAL_STRING("c", arr[2]);
    TEST_ASSERT_NULL(arr[3]);
    for (int i = 0; arr[i]; ++i)
        free(arr[i]);
    free(arr);
}

// Тест strtokarr - NULL на входе
void test_strtokarr_null(void)
{
    TEST_ASSERT_NULL(strtokarr(NULL, ';'));
}

// Тест strtokarr - пустая строка
void test_strtokarr_empty_string(void)
{
    char **arr = strtokarr("", ';');
    TEST_ASSERT_NOT_NULL(arr);
    TEST_ASSERT_NULL(arr[0]);
    free(arr);
}

// Тест strtokarr - строка без разделителей
void test_strtokarr_no_delimiter(void)
{
    char **arr = strtokarr("abc", ';');
    TEST_ASSERT_NOT_NULL(arr);
    TEST_ASSERT_EQUAL_STRING("abc", arr[0]);
    TEST_ASSERT_NULL(arr[1]);
    for (int i = 0; arr[i]; ++i)
        free(arr[i]);
    free(arr);
}
