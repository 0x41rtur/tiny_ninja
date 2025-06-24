#include "fs.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "clip.h"
#include "common.h"

#ifndef MAX_DEPTH
#define MAX_DEPTH 256
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/// Проверяет, является ли файл целевым (по расширению и типу).
///
/// Условия:
/// - Файл должен существовать и быть обычным (`regular file`);
/// - Расширение должно совпадать с `target`.
///
/// Параметры:
/// - `filename`: имя файла (строка, обязательна);
/// - `target`: требуемое расширение (без точки, например, `"txt"`).
///
/// Возвращает:
/// - `0`, если файл подходит;
/// - `-1`, если:
///     - файл не существует;
///     - файл не является обычным;
///     - расширение отсутствует или не совпадает.
///
/// Выделенная память внутри будет автоматически освобождена.
int
is_target(const char *filename, const char *target)
{
        if (0 != is_regular_file(filename))
        {
                return -1;
        }
        const char *ext = find_ext_suffix(filename);
        if (NULL == ext)
        {
                return -1;
        }
        if (0 != strcmp(ext, target))
        {
                free((void *) ext);
                return -1;
        }
        free((void *) ext);
        return 0;
}

/// Ищет все файлы с заданным расширением в текущей директории.
///
/// Алгоритм:
/// - Открывает текущую директорию;
/// - Проверяет каждый файл через `is_target`;
/// - Для подходящих:
///     - создаёт структуру `target`;
///     - копирует `name` и дублирует команду `cmd`;
/// - Возвращает NULL-терминированный массив указателей на `target`.
///
/// Параметры:
/// - `cmd`: команда, содержащая фильтрующее расширение (`cmd->ext`);
///
/// Возвращает:
/// - NULL, если ни один файл не подошёл или произошла ошибка;
/// - Указатель на массив `struct target*`, последний элемент — `NULL`.
///
/// Примечания:
/// - Использует `exit(EXIT_FAILURE)` при фатальной ошибке;
/// - Возвращаемый массив и все структуры внутри требуют явного освобождения.
///
/// Требует:
/// - Реализации `copy_command()`;
/// - Правильного `cmd->ext` (например, `"txt"`).
__attribute__((malloc))
struct target **
find_target(const struct command *cmd)
{
        DIR *current_dir = opendir(".");
        if (NULL == current_dir)
        {
                // open_dir_error(".");
                exit(EXIT_FAILURE);
        }
        size_t               count    = 0;
        size_t               capacity = 1;
        struct target **        entries  = NULL;
        const struct dirent *entry    = NULL;
        while (NULL != (entry = readdir(current_dir)))
        {
                if (-1 == is_target(entry->d_name, cmd->ext))
                {
                        continue;
                }
                if (NULL == entries || count == capacity)
                {
                        capacity *= 2;
                        struct target **new_entries = (struct target **) realloc(
                                (void *) entries,
                                sizeof(struct target *) * capacity);
                        if (NULL == new_entries)
                        {
                                // memory_error();
                                exit(EXIT_FAILURE);
                        }
                        entries = new_entries;
                }
                struct target *target = malloc(sizeof(struct target));
                if (NULL == target)
                {
                        // memory_error();
                        exit(EXIT_FAILURE);
                }
                target->name = strcopy(entry->d_name);
                int error = 0;
                target->cmd  = copy_command(&error, cmd);
                if (NULL == target->cmd || NULL == target->name)
                {
                        // memory_error();
                        exit(EXIT_FAILURE);
                }
                entries[count] = target;
                ++count;

        }
        if (NULL == entries || 0 == count)
        {
                // no_matching_files_error(cmd->ext);
                return NULL;
        }
        struct target **new_entries = (struct target **) realloc((void *) entries,
                sizeof(struct target *) * (count + 1));
        if (NULL == new_entries)
        {
                // memory_error();
                exit(EXIT_FAILURE);
        }
        entries        = new_entries;
        entries[count] = NULL;
        return entries;
}

/// Создаёт директорию, если она ещё не существует.
///
/// Параметры:
/// - `dir`: путь до директории.
///
/// Возвращает:
/// - `0`, если директория создана успешно;
/// - `1`, если директория уже существует (`EEXIST`);
/// - `-1`, если произошла другая ошибка (`errno` сохранится).
int
mk_dir(const char *dir)
{
        const int status = mkdir(dir, 0777);
        if (-1 == status && errno != EEXIST)
        {
                return -1;
        }
        return -1 == status ? 1 : 0;
}

/// Рекурсивно создаёт вложенные директории (относительно текущей папки).
///
/// Алгоритм:
/// - Разбивает путь по `/`;
/// - Поэтапно наращивает путь и вызывает `mk_dir`;
/// - В случае ошибки — удаляет все ранее созданные директории.
///
/// Параметры:
/// - `dir`: путь вида `a/b/c`, где `a`, `b` и `c` будут созданы по порядку.
///
/// Возвращает:
/// - `0`, если создан хотя бы один каталог;
/// - `1`, если все каталоги уже существовали;
/// - `-1`, если произошла ошибка и выполнен откат.
///
/// Примечания:
/// - Не поддерживает абсолютные пути (начинающиеся с `/`);
/// - Память освобождается автоматически.
/// - Максимальная вложенность ограничена `MAX_DEPTH`.
int
make_dir_recursive(const char *dir)
{
        char *  copy      = strcopy(dir);
        // FIXME: все пути как оносительно текущей директории поэтому для тупи
        // /abc что означает от корня мы создаим не от корня и это нормально
        const char *  part_path = strtok(copy, "/");
        char    cur_path[PATH_MAX] = {'\0'};
        char *  stack_created_paths[MAX_DEPTH] = {NULL};
        ssize_t stack_top = 0;
        while (NULL != part_path)
        {
                // просто убедимся что массив не пустой
                // для пустого маасива добавлени / означалобы что создает
                // относителя корня что не верно в, мы работаем тольк с относительно вложенными каталогами
                if (cur_path[0] != '\0')
                {
                        strcat(cur_path, "/");
                }
                strcat(cur_path, part_path);
                const int stat = mk_dir(cur_path);
                if (-1 == stat)
                {
                        while (--stack_top >= 0)
                        {
                                rmdir(stack_created_paths[stack_top]);
                                free(stack_created_paths[stack_top]);
                        }
                        free(copy);
                        return -1;
                }
                if (0 == stat)
                {
                        stack_created_paths[stack_top++] = strcopy(cur_path);
                }
                part_path = strtok(NULL, "/");
        }
        free(copy);
        if (0 == stack_top)
        {
                return 1;
        }
        while (--stack_top >= 0)
        {
                free(stack_created_paths[stack_top]);
        }
        return 0;
}
