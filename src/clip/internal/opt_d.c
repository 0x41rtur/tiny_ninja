#include "opt_d.h"

#include "common.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

/// Проверяет, является ли строка системным обозначением директории: "." или "..".
/// @param arg Строка с именем сегмента пути.
/// @return 1, если `arg` равно "." или "..", иначе 0.
static int
is_sys_dir_name(const char *arg)
{
        return 0 == strcmp(arg, ".") || 0 == strcmp(arg, "..") ? 1 : 0;
}

/// Проверяет, допустим ли путь как строка: отсутствие недопустимых символов и шаблонов.
/// Отклоняет:
/// - корень `/`
/// - `.` и `..`
/// - пустую строку
/// - строки, содержащие `//`, `./`, `/./`, пробелы
///
/// @param arg строка с путем.
/// @return 1, если строка допустима, иначе 0.
static int
is_permissible_sequence(const char *arg)
{
        if (1 == is_sys_dir_name(arg) || 0 == strcmp(arg, "/") ||
            ((strncmp(arg, "./", 2) == 0)) || ((strncmp(arg, "//", 2) == 0)) ||
            ((strstr(arg, "//") != NULL)))
        {
                return 0;
        }
        const size_t len = strlen(arg);
        size_t       i   = 1;
        while ('\0' != arg[i])
        {
                if ('/' == arg[i] && '/' == arg[i - 1])
                {
                        return 0;
                }
                ++i;
        }
        for (size_t j = 0; j < len; j++)
        {
                if (0 == isspace(arg[j]))
                {
                        return 1;
                }
        }
        return 0;
}

/// Проверяет корректность значения аргумента `-d`
///
/// Допустимы только валидные относительные или абсолютные пути, без:
/// - ".", ".."
/// - `//`
/// - `./`
/// - строк, содержащих только пробелы
///
/// Каждый сегмент пути (через `/`) проверяется отдельно:
/// - не должен быть `.` или `..`
/// - не должен превышать `NAME_MAX`
///
/// @param arg Строка с путем.
/// @return 0 если путь валиден, иначе -1.
int
argd(const char *arg)
{
        if (NULL == arg)
        {
                return -1;
        }
        const size_t len = strlen(arg);
        if (0 == len)
        {
                return -1;
        }
        if (0 == is_permissible_sequence(arg))
        {
                return -1;
        }
        char **arr_tok = strtokarr(arg, '/');
        if (NULL == arr_tok)
        {
                return -1;
        }
        if (NULL != *arr_tok && 0 == strcmp(*arr_tok, ""))
        {
                ++arr_tok;
        }
        while (NULL != *arr_tok)
        {
                if (1 == is_sys_dir_name(*arr_tok) || len >= NAME_MAX)
                {
                        return -1;
                }
                ++arr_tok;
        }
        return 0;
}
