/**
 * @file fs.c
 * @brief Утилиты для работы файловой системой.
 */

#include "fs.h"

#include "common/console.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "clip/clip.h"
#include "common/common.h"

/**
 * @brief Проверяет, является ли файл TNT-файлом нужного расширения.
 *
 * @param filename Имя файла
 * @param target Расширение без точки (например, "jpg")
 * @return 0 — если файл с нужным расширением, -1 — иначе
 */
int
is_tnt(const char *filename, const char *target)
{
        if (0 != is_file(filename))
        {
                return -1;
        }
        const char *ext = extension(filename);
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

/**
 * @brief Находит все файлы с нужным расширением в текущей директории
 *
 * @param cmd Команда с расширением
 * @return NULL-терминированный массив `struct tnt *`. При ошибке — аварийный выход.
 */
__attribute__((malloc))
struct tnt **
find_tnt(const struct command *cmd)
{
        DIR *current_dir = opendir(".");
        if (NULL == current_dir)
        {
                open_dir_error(".");
                exit(EXIT_FAILURE);
        }
        size_t               count    = 0;
        size_t               capacity = 1;
        struct tnt **        entries  = NULL;
        const struct dirent *entry    = NULL;
        while (NULL != (entry = readdir(current_dir)))
        {
                if (-1 == is_tnt(entry->d_name, cmd->ext))
                {
                        continue;
                }
                if (NULL == entries || count == capacity)
                {
                        capacity *= 2;
                        struct tnt **new_entries = (struct tnt **) realloc(
                                (void *) entries,
                                sizeof(struct tnt *) * capacity);
                        if (NULL == new_entries)
                        {
                                memory_error();
                                exit(EXIT_FAILURE);
                        }
                        entries = new_entries;
                }
                struct tnt *target = malloc(sizeof(struct tnt));
                if (NULL == target)
                {
                        memory_error();
                        exit(EXIT_FAILURE);
                }
                target->name = copy_str(entry->d_name);
                target->cmd  = copy_command(cmd);
                if (NULL == target->cmd || NULL == target->name)
                {
                        memory_error();
                        exit(EXIT_FAILURE);
                }
                entries[count] = target;
                ++count;

        }
        if (NULL == entries || 0 == count)
        {
                no_matching_files_error(cmd->ext);
                return NULL;
        }
        struct tnt **new_entries = (struct tnt **) realloc((void *) entries,
                sizeof(struct tnt *) * (count + 1));
        if (NULL == new_entries)
        {
                memory_error();
                exit(EXIT_FAILURE);
        }
        entries        = new_entries;
        entries[count] = NULL;
        return entries;
}

/**
 * @brief Создаёт директорию, если она ещё не существует.
 *
 * Проверяет существование директории по указанному пути `dir`.
 * Если директория не существует — пытается создать её с правами `0777`.
 * Если директория уже существует — считается успешным результатом.
 *
 * @param dir Путь к директории, которую нужно создать.
 * @return 0 директория была создана,
 *         1 директория существовала
 *         -1 при ошибке (например, недостаточно прав или неверный путь).
 *
 * @note Для корректной работы необходимы права на чтение текущего каталога
 *       и на создание подкаталогов, если директория не существует.
 *
 * @warning Функция не проверяет, является ли `dir` ссылкой на обычный файл
 *          или некорректной сущностью — `mkdir()` может завершиться с ошибкой.
 */
int
mk_dir(const char *dir)
{
        int status = 1;
        DIR *open = opendir(dir);
        if (NULL == open)
        {
                if (-1 == mkdir(dir, 0777))
                {
                        if (EEXIST == errno)
                        {
                                status = 1;
                        } else
                        {
                                status = -1;
                        }
                } else
                {
                        status = 0;
                }
        }
        closedir(open);
        return status;
}

int rm_dir(const char *dir)
{
        if (NULL == dir)
        {
                return -1;
        }
        return rmdir(dir);
}
