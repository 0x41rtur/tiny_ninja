/**
 * @file clip.c
 * @brief Разбор аргументов командной строки и построение сопоставлений расширений с директориями.
 *
 * Поддерживает ключи:
 * - `-e <ext>` и `-d <dir>` — одиночная пара расширения и директории.
 * - `-m <map>` — множественное сопоставление в формате `ext=dir;ext2=dir2`.
 * - `-h` — вывод справки.
 *
 * Применяется в CLI-утилитах, которым необходимо сопоставлять типы файлов с директориями.
 */

#include "clip.h"

#include "common/console.h"
#include "common/common.h"

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Проверяет, допустимо ли расширение.
 *
 * Расширение считается корректным, если оно непустое и состоит
 * только из буквенно-цифровых символов.
 *
 * @param ext Строка расширения (например, "jpg")
 * @return 1 если корректно, 0 иначе
 */
static int
is_valid_ext(const char *ext)
{
        if (NULL == ext || 0 == strlen(ext))
        {
                return 0;
        }
        const char *ch = ext;
        while ('\0' != *ch)
        {
                if (!isalnum(*ch))
                {
                        return 0;
                }
                ++ch;
        }
        return 1;
}

/**
 * @brief Проверяет, допустим ли путь к директории.
 *
 * Путь считается корректным, если он непустой и содержит
 * только буквенно-цифровые символы и пробелы.
 *
 * @param path Путь к директории
 * @return 1 если корректно, 0 иначе
 */
static int
is_valid_dir(const char *path)
{
        if (NULL == path || 0 == strlen(path))
        {
                return 0;
        }
        const char *ch = path;
        while ('\0' != *ch)
        {
                if (!isalnum(*ch) && !isspace(*ch))
                {
                        return 0;
                }
                ++ch;
        }
        return 1;
}

/**
 * @brief Разбирает карту вида "ext1=dir1;ext2=dir2".
 *
 * Каждая пара разделяется символом `=`, пары — `;`.
 * Если хотя бы одна пара некорректна, разбор отменяется (атомарность).
 *
 * @param map Строка карты расширений
 * @return Завершённый `NULL` массив `struct command*`, или NULL при ошибке
 */
__attribute__((malloc))
static const struct command **
map_cmd(const char *map)
{
        if (NULL == map)
        {
                return NULL;
        }
        char *copy = copy_str(map);
        if (NULL == copy)
        {
                return NULL;
        }
        const struct command **cmds     = NULL;
        size_t                 count    = 0;
        size_t                 capacity = 1;
        const char *           pair     = strtok(copy, ";");
        while (NULL != pair)
        {
                char *ext = NULL;
                char *dir = NULL;
                if (-1 == split(pair, &ext, &dir, '=') ||
                    !is_valid_ext(ext) || !is_valid_dir(dir))
                {
                        free(ext);
                        free(dir);
                        free(copy);
                        return NULL;
                }
                struct command *cmd = malloc(sizeof(struct command));
                if (NULL == cmd)
                {
                        memory_error();
                        exit(EXIT_FAILURE);
                }
                cmd->ext = ext;
                cmd->dir = dir;
                if (count == capacity || cmds == NULL)
                {
                        capacity *= 2;
                        cmds = realloc(
                                cmds, capacity * sizeof(struct command *));
                        if (NULL == cmds)
                        {
                                memory_error();
                                exit(EXIT_FAILURE);
                        }
                }
                cmds[count++] = cmd;
                pair          = strtok(NULL, ";");
        }
        if (count == 0)
        {
                free(copy);
                return NULL;
        }
        cmds = realloc(cmds, (count + 1) * sizeof(struct command *));
        if (NULL == cmds)
        {
                memory_error();
                exit(EXIT_FAILURE);
        }
        cmds[count] = NULL;
        free(copy);
        return cmds;
}

/**
 * @brief Обрабатывает аргументы командной строки и формирует массив команд.
 *
 * Поддерживаемые опции:
 * - `-e <ext>`: расширение (вместе с `-d`)
 * - `-d <dir>`: путь к директории
 * - `-m <map>`: множественное сопоставление `ext=dir;...`
 * - `-h`: выводит справку
 *
 * Возвращает массив `struct command*`, завершённый `NULL`.
 * При ошибках завершает выполнение программы.
 *
 * @param argc Количество аргументов
 * @param argv Массив аргументов
 * @return Массив команд `struct command*`, заканчивающийся `NULL`
 */
__attribute__((malloc))
const struct command **
clip(const int argc, char **argv)
{
        if (argc < 2)
        {
                clip_info();
                exit(EXIT_FAILURE);
        }
        const struct command **cmds      = NULL;
        const char *           extension = NULL;
        const char *           directory = NULL;
        opterr                           = 0;
        int opt                          = 0;
        while ((opt = getopt(argc, argv, "e:d:m:h")) != -1)
        {
                switch (opt)
                {
                case 'e':
                        if (!is_valid_ext(optarg))
                        {
                                clip_e_error();
                                exit(EXIT_FAILURE);
                        }
                        extension = optarg;
                        break;
                case 'd':
                        if (!is_valid_dir(optarg))
                        {
                                clip_d_error();
                                exit(EXIT_FAILURE);
                        }
                        directory = optarg;
                        break;
                case 'm':
                        cmds = map_cmd(optarg);
                        if (NULL == cmds)
                        {
                                clip_m_error();
                                exit(EXIT_FAILURE);
                        }
                        break;
                default:
                        clip_info();
                        exit(EXIT_FAILURE);
                }
        }
        if (extension && directory)
        {
                struct command *cmd = malloc(sizeof(struct command));
                if (NULL == cmd)
                {
                        memory_error();
                        exit(EXIT_FAILURE);
                }
                cmd->ext = copy_str(extension);
                cmd->dir = copy_str(directory);
                if (NULL == cmd->ext || NULL == cmd->dir)
                {
                        memory_error();
                        exit(EXIT_FAILURE);
                }
                cmds = malloc(2 * sizeof(struct command *));
                if (NULL == cmds)
                {
                        memory_error();
                        exit(EXIT_FAILURE);
                }
                cmds[0] = cmd;
                cmds[1] = NULL;
                return cmds;
        }
        if (NULL != cmds)
        {
                return cmds;
        }
        if (NULL == extension)
        {
                clip_e_error();
        }
        if (NULL == directory)
        {
                clip_d_error();
        }
        exit(EXIT_FAILURE);
}

/**
 * @brief Создаёт глубокую копию команды.
 *
 * Выделяет новую структуру и копирует все строки.
 * При ошибке завершает выполнение программы.
 *
 * @param cmd Исходная команда
 * @return Новая команда (указатель), либо NULL если вход NULL
 */
__attribute__((malloc))
struct command *
copy_command(const struct command *cmd)
{
        if (NULL == cmd)
        {
                return NULL;
        }
        struct command *copy = malloc(sizeof(struct command));
        if (NULL == copy)
        {
                memory_error();
                exit(EXIT_FAILURE);
        }
        copy->ext = copy_str(cmd->ext);
        copy->dir = copy_str(cmd->dir);
        if (NULL == copy->ext || NULL == copy->dir)
        {
                memory_error();
                exit(EXIT_FAILURE);
        }
        return copy;
}
