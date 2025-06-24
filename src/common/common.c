#include "common.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdarg.h>

#ifndef PATH_MAX
#define PATH_MAX 4096 // fallback, POSIX минимум
#endif

/// Создаёт динамическую копию строки.
///
/// \param s
///     Входная строка для копирования. Если NULL, функция возвращает NULL.
///
/// \return
///     Указатель на новую строку в динамической памяти,
///     идентичную входной. Требуется освободить с помощью free() после использования.
///     При ошибке выделения памяти или если вход NULL — возвращает NULL.
///
/// \note
///     - Копируется вся строка вместе с завершающим нулём.
///     - Выделение памяти происходит через malloc.
///
/// \par Преимущества
///     - Простая и надёжная реализация.
///     - Не зависит от сторонних библиотек.
///
/// \par Ограничения и недостатки
///     - Возвращаемый указатель требует явного освобождения памяти.
///     - Не проверяет выход за пределы, так как работает со стандартной strlen.
///     - При входе NULL возвращает NULL без ошибки.
///
/// \par Использование
///     char *copy = strcopy("hello");
///     if (copy) {
///         // работа со строкой copy
///         free(copy);
///     }
__attribute__((malloc)) char *
strcopy(const char *s)
{
        if (NULL == s)
        {
                return NULL;
        }
        const size_t len  = strlen(s) + 1;
        char        *copy = malloc(len);
        if (NULL == copy)
        {
                return NULL;
        }
        memcpy(copy, s, len);
        return copy;
}

/// Создаёт динамическую копию первых `l` символов строки `s`.
///
/// \param s
///     Входная строка для копирования. Если NULL, функция возвращает NULL.
/// \param l
///     Количество символов для копирования из строки `s`.
///
/// \return
///     Указатель на новую строку, содержащую первые `l` символов `s` с нуль-терминатором.
///     Память выделена динамически, требуется освободить через `free()`.
///     При ошибке выделения памяти или если `s` равна NULL — возвращает NULL.
///
/// \note
///     - Функция копирует ровно `l` байт из `s`, без проверки длины исходной строки.
///     - Добавляет нуль-терминатор в конец результирующей строки.
///     - Если длина `l` больше длины строки `s`, возможен выход за границы памяти (небезопасно).
///
/// \par Преимущества
///     - Быстрая копия фиксированного количества символов.
///     - Простая реализация с явным добавлением нуль-терминатора.
///
/// \par Ограничения и недостатки
///     - Не проверяет длину строки `s`, поэтому необходимо убедиться, что `l` не превышает длину `s`.
///     - Возвращаемый указатель требует явного освобождения.
///     - При входе NULL возвращает NULL без ошибки.
///
/// \par Использование
///     char *prefix = strncopy("hello world", 5);
///     if (prefix) {
///         // prefix == "hello"
///         free(prefix);
///     }
__attribute__((malloc)) char *
strncopy(const char *s, const size_t l)
{
        if (NULL == s)
        {
                return NULL;
        }
        char *result = malloc(l + 1); // +1 под нуль-терминатор
        if (NULL == result)
        {
                return NULL;
        }
        memcpy(result, s, l);
        result[l] = '\0';
        return result;
}

/// Разбивает строку `s` на две части по первому вхождению разделителя `d`.
///
/// \param s
///     Входная строка для разбивки. Если `NULL`, функция возвращает `-1`.
/// \param before
///     Указатель на переменную, в которую будет записан указатель на новую строку
///     с частью до разделителя (не включая разделитель).
/// \param after
///     Указатель на переменную, в которую будет записан указатель на новую строку
///     с частью после разделителя (без разделителя).
/// \param d
///     Символ-разделитель, по которому происходит разбиение.
///
/// \return
///     `0` — успешное разбиение и выделение памяти для обеих частей;
///     `-1` — при ошибке (например, `s == NULL`, разделитель не найден или не удалось выделить память).
///
/// \note
///     - Память для `*before` и `*after` выделяется динамически и должна быть освобождена вызывающей стороной.
///     - Если разделитель не найден, функция возвращает `-1`.
///     - В случае ошибки выделения памяти возвращает `-1` и не гарантирует освобождение памяти.
///
/// \par Преимущества
///     - Простая и понятная логика разбиения строки.
///     - Использует безопасное копирование с выделением памяти.
///
/// \par Ограничения и недостатки
///     - Не освобождает ранее выделенную память при ошибке — возможен утечка, если вызывающий не обработает.
///     - Использует `strchr`, что может быть менее эффективно для очень больших строк.
///     - Требует проверять возврат и освобождать память для обеих частей.
///
/// \par Использование
///     char *key = NULL;
///     char *value = NULL;
///     if (split("key=value", &key, &value, '=') == 0) {
///         // key == "key", value == "value"
///         free(key);
///         free(value);
///     }
int
split(const char *s, char **before, char **after, const char d)
{
        if (NULL == s)
        {
                return -1;
        }
        const char *pos = strchr(s, d);
        if (NULL != pos)
        {
                const size_t key_len = (size_t) pos - (size_t) s;
                *before              = strncopy(s, key_len);
                *after               = strcopy(pos + 1);
                if (NULL == *after || NULL == *before)
                {
                        return -1;
                }
                return 0;
        }
        return -1;
}

/// Возвращает копию расширения файла после последней точки.
///
/// \param filename
///     Имя файла или путь. Если `NULL`, возвращается `NULL`.
///
/// \return
///     Указатель на новую строку с расширением файла (без точки), если оно найдено.
///     Если расширение отсутствует, строка заканчивается на точку или `filename == NULL`, возвращается `NULL`.
///
/// \note
///     - Функция ищет последнюю точку в имени файла и копирует всё после неё.
///     - Память выделяется через `malloc`, необходимо вызвать `free()` после использования.
///
/// \par Пример:
///     find_ext_suffix("archive.tar.gz") → "gz"
///
/// \par Ограничения:
///     - Не различает пути и базовые имена (например, не работает с basename()).
///     - Не считает "точку" в начале имени файла как расширение.
__attribute__((malloc)) char *
find_ext_suffix(const char *filename)
{
        if (NULL == filename)
        {
                return NULL;
        }
        size_t tail = strlen(filename) - 1;
        if (filename[tail] == '.')
        {
                return NULL;
        }
        --tail;
        while (0 < tail)
        {
                if (filename[tail] == '.')
                {
                        return strcopy(filename + tail + 1);
                }
                --tail;
        }
        return NULL;
}

/// Проверяет, является ли файл регулярным.
///
/// \param filename
///     Путь к файлу. Не должен быть `NULL`.
///
/// \return
///     - `0` — если файл существует и является обычным (регулярным).
///     - `1` — если файл существует, но не регулярный (например, директория).
///     - `-1` — если файл не существует или произошла ошибка `stat()`.
///
/// \note
///     - Использует системный вызов `stat`.
///     - Не различает симлинки — для проверки симлинков нужно использовать `lstat`.
int
is_regular_file(const char *filename)
{
        struct stat st;
        if (0 != stat(filename, &st))
        {
                return -1;
        }
        return S_ISREG(st.st_mode) ? 0 : 1;
}

/// Объединяет произвольное количество строк в одну строку.
///
/// \param first
///     Первая строка (обязательная). Если `NULL`, функция возвращает `NULL`.
///
/// \param ...
///     Переменное количество `const char*`, заканчивающееся `NULL`.
///
/// \return
///     Новая строка, содержащая конкатенацию всех строк.
///     Память выделяется через `malloc`, необходимо освободить через `free()`.
///     Возвращает `NULL`, если `first == NULL` или при ошибке выделения.
///
/// \note
///     - Строки могут быть пустыми, `NULL` в середине обрабатываются.
///     - Если строка внутри списка `NULL`, она игнорируется.
///
/// \par Пример:
///     concat("Hello", " ", "world", "!", NULL) → "Hello world!"
///
/// \par Ограничения:
///     - Требует аккуратного завершения списка `NULL`.
///     - Не потокобезопасна (из-за `va_list`).
__attribute__((malloc)) char *
concat(const char *first, ...)
{
        if (NULL == first)
        {
                return NULL;
        }
        va_list args;
        va_start(args, first);
        size_t      total_len = strlen(first);
        const char *s         = NULL;
        va_list     args_copy;
        va_copy(args_copy, args);
        while (NULL != (s = va_arg(args_copy, const char *)))
        {
                total_len += strlen(s);
        }
        va_end(args_copy);
        char *res = malloc(total_len + 1);
        if (NULL == res)
        {
                va_end(args);
                return NULL;
        }
        if (NULL == strcpy(res, first))
        {
                va_end(args);
                free(res);
                return NULL;
        }
        while (NULL != (s = va_arg(args, char *)))
        {
                if (NULL == strcat(res, s))
                {
                        va_end(args);
                        free(res);
                        return NULL;
                }
        }
        va_end(args);
        return res;
}

/// Разбивает строку `str` на массив подстрок, используя символ-разделитель `delim`.
///
/// \param str
///     Исходная строка для разбора. Если `NULL`, возвращается `NULL`.
///
/// \param delim
///     Символ, по которому происходит разделение (например, `';'`).
///
/// \return
///     Указатель на массив указателей на строки, заканчивающийся `NULL`.
///     Все строки и сам массив выделяются через `malloc`, необходимо освободить вручную.
///     При ошибке возвращается `NULL`.
///
/// \note
///     - Память: каждый элемент `arr[i]` и сам `arr` нужно освобождать.
///     - Работает корректно даже если в строке нет `delim`.
///
/// \par Пример:
///     strtokarr("one;two;three", ';') → { "one", "two", "three", NULL }
///
/// \par Ограничения:
///     - Не обрабатывает пустые элементы (например, `a;;b` → `"a"`, `"b"`, без пустого).
///     - При ошибке освобождает всё, что уже было выделено.
///     - Не модифицирует входную строку (`strtok` делает это).
char **
strtokarr(const char *str, int delim)
{
        if (NULL == str)
        {
                return NULL;
        }
        const size_t len  = strlen(str);
        size_t       i    = 0;
        size_t       size = 0;
        while (i < len)
        {
                if (str[i] == delim)
                {
                        ++size;
                }
                ++i;
        }
        char **arr = malloc(sizeof(char *) * (size + 1));
        if (NULL == arr)
        {
                return NULL;
        }
        size_t arr_pos  = 0;
        size_t curr_pos = 0;
        size_t next_pos = 0;
        while (curr_pos < len)
        {
                if (str[curr_pos] == delim)
                {
                        char *tok =
                            strncopy(str + next_pos, curr_pos - next_pos);
                        if (NULL == tok)
                        {
                                for (size_t k = 0; k < curr_pos; k++)
                                {
                                        free(arr[k]);
                                }
                                free(arr);
                                return NULL;
                        }
                        next_pos     = curr_pos + 1;
                        arr[arr_pos] = tok;
                        ++arr_pos;
                }
                ++curr_pos;
        }
        char *tok = strncopy(str + next_pos, curr_pos - next_pos);
        if (NULL != tok)
        {
                arr[arr_pos] = tok;
                ++arr_pos;
        }
        arr[arr_pos] = NULL;
        return arr;
}
