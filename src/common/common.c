/**
 * @file common.c
 * @brief Утилиты для работы со строками и путями.
 */

#include "common.h"

#include <limits.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdarg.h>

#ifndef PATH_MAX
#define PATH_MAX 4096  // fallback, POSIX минимум
#endif

/**
 * @brief Копирует строку в новую область памяти.
 *
 * Создаёт дубликат строки `s` в куче.
 *
 * @param s Указатель на исходную строку
 * @return Указатель на новую строку или NULL при ошибке
 */
__attribute__((malloc))
char *
copy_str(const char *s)
{
        if (NULL == s)
        {
                return NULL;
        }
        const size_t len  = strlen(s) + 1;
        char *       copy = malloc(len);
        if (NULL == copy)
        {
                return NULL;
        }
        memcpy(copy, s, len);
        return copy;
}

/**
 * @brief Копирует первые `l` байт строки и завершает результат `\0`.
 *
 * Полезно при копировании подстрок без ручного добавления нуль-терминатора.
 *
 * @param s Указатель на исходную строку
 * @param l Количество байт для копирования
 * @return Указатель на новую строку или NULL при ошибке
 */
__attribute__((malloc))
char *
copy_str_n(const char *s, const size_t l)
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

/**
 * @brief Разделяет строку на две части по первому вхождению символа.
 *
 * Находит первый символ-разделитель `d` в строке `s`, и создаёт две новые строки:
 * до и после разделителя.
 *
 * @param s Исходная строка
 * @param before [out] Адрес, куда записывается указатель на строку до разделителя
 * @param after  [out] Адрес, куда записывается указатель на строку после разделителя
 * @param d Символ-разделитель
 * @return 0 при успехе, -1 при ошибке или если разделитель не найден
 *
 * @note Выделенная память в `*before` и `*after` должна быть освобождена вручную.
 */
int
split(const char *s, char **before, char **after, const char d)
{
        const char *pos = strchr(s, d);
        if (NULL != pos)
        {
                const size_t key_len = (size_t) pos - (size_t) s;
                *before              = copy_str_n(s, key_len);
                *after               = copy_str(pos + 1);
                if (NULL == *after || NULL == *before)
                {
                        return -1;
                }
                return 0;
        }
        return -1;
}

/**
 * @brief Извлекает расширение файла из имени.
 *
 * Возвращает копию подстроки после последней точки в имени файла.
 *
 * Примеры:
 * - `"file.txt"` → `"txt"`
 * - `"archive.tar.gz"` → `"gz"`
 * - `"noext"` → NULL
 * - `"dot."` → NULL
 *
 * @param filename Имя файла
 * @return Копия расширения (без точки) или NULL, если не найдено
 *
 * @note Память под расширение выделяется динамически и должна быть освобождена.
 */
__attribute__((malloc))
char *
extension(const char *filename)
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
                        return copy_str(filename + tail + 1);
                }
                --tail;
        }
        return NULL;
}

/**
 * @brief Проверяет тип пути.
 *
 * @param filename Путь к проверяемому объекту
 * @return:
 *     0 — это обычный файл,
 *     1 — это не файл (например, директория, сокет и пр.),
 *    -1 — ошибка (например, нет доступа, не существует и пр.)
 */
int
is_file(const char *filename)
{
        struct stat st;
        if (0 != stat(filename, &st))
        {
                return -1;
        }
        return S_ISREG(st.st_mode) ? 0 : 1;
}

/**
 * @brief Конкатенирует произвольное количество строк в одну.
 *
 * Создаёт новую строку, содержащую все строки, переданные через аргументы,
 * объединённые в порядке следования. Память под результирующую строку
 * выделяется динамически и должна быть освобождена вызывающей стороной.
 *
 * Функция принимает хотя бы одну строку `first` и произвольное количество
 * последующих строк, завершаемых `NULL`. Пример использования:
 *
 * @code
 * char *result = concat("usr", "/", "bin", "/", "env", NULL);
 * // result = "usr/bin/env"
 * free(result);
 * @endcode
 *
 * @note Поведение не определено, если в аргументах (до NULL) передана `NULL`-строка.
 *
 * @param first Первая строка (не может быть NULL).
 * @param ... Остальные строки, заканчивающиеся NULL.
 * @return Указатель на новую строку или NULL при ошибке (например, нехватка памяти).
 */
__attribute__((malloc))
char *
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
        while (NULL != (s = va_arg(args_copy, const char*)))
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
        while (NULL != (s = va_arg(args, char*)))
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
