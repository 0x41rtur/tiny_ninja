#include "clip.h"

#include "opt_d.h"
#include "opt_e.h"
#include "opt_m.h"

#include "common.h"

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Разбирает аргументы командной строки и возвращает массив структур `command`.
///
/// Поддерживает флаги:
///   - `-e <ext>` — расширение файла (валидируется через `arge`)
///   - `-d <dir>` — директория назначения (валидируется через `argd`)
///   - `-m <map>` — маппинг `ext=dir;...` (обрабатывается через `argm`)
///
/// Варианты:
///   - Если указан `-m`, возвращает массив из `argm`
///   - Если указан `-e` и `-d`, создаёт массив вручную
///
/// \param[out] error Указатель для возврата кода ошибки (CLIP_OK, CLIP_ERR_BAD_E_OPT и т.д.)
/// \param[in] argc Количество аргументов
/// \param[in] argv Массив аргументов
/// \return NULL при ошибке, либо массив `struct command *`, завершённый NULL
__attribute__((malloc)) const struct command **
clip(int *error, const int argc, char **argv)
{
        if (argc < 2 || NULL == argv || NULL == *argv)
        {
                *error = CLIP_ERR_BAD_OPT;
                return NULL;
        }
        optarg          = NULL;
        opterr          = 0;
        optopt          = 0;
        optind          = 1;
        *error          = CLIP_OK;
        char *extension = NULL;
        char *directory = NULL;
        int   opt       = 0;
        while (-1 != (opt = getopt(argc, argv, "e:d:m:h")))
        {
                switch (opt)
                {
                case 'e':
                        if (NULL != extension || -1 == arge(optarg))
                        {
                                *error = CLIP_ERR_BAD_E_OPT;
                                return NULL;
                        }
                        extension = optarg;
                        break;
                case 'd':
                        if (NULL != directory || -1 == argd(optarg))
                        {
                                *error = CLIP_ERR_BAD_D_OPT;
                                return NULL;
                        }
                        directory = optarg;
                        break;
                case 'm':
                {
                        int                    e = ARGM_OK;
                        const struct command **c = argm(&e, optarg);
                        if (NULL == c || NULL == *c)
                        {
                                *error = e == ARGM_MEM_ERR ? CLIP_PANIC
                                                           : CLIP_ERR_BAD_M_OPT;
                                return NULL;
                        }
                        return c;
                }
                default:
                        *error = CLIP_UNEXPECTED_OPT;
                        return NULL;
                }
        }
        if (NULL != extension && NULL != directory)
        {
                struct command *cmd = malloc(sizeof(struct command));
                if (NULL == cmd)
                {
                        *error = CLIP_PANIC;
                        goto inner_cleanup;
                }
                cmd->ext = strcopy(extension);
                cmd->dir = strcopy(directory);
                if (NULL == cmd->ext || NULL == cmd->dir)
                {
                        *error = CLIP_PANIC;
                        goto inner_cleanup;
                }
                const struct command **cmds =
                    malloc(2 * sizeof(struct command *));
                if (NULL == cmds)
                {
                        *error = CLIP_PANIC;
                        goto inner_cleanup;
                }
                cmds[0] = cmd;
                cmds[1] = NULL;
                return cmds;
        inner_cleanup:
                if (NULL != cmd)
                {
                        free((void *) cmd->dir);
                        free((void *) cmd->ext);
                        free(cmd);
                }
                return NULL;
        }
        *error = CLIP_UNEXPECTED_OPT;
        return NULL;
}

/// Копирует структуру `command`.
///
/// Выделяет новую структуру и копирует поля `ext` и `dir`.
///
/// \param[out] error Указатель на код ошибки (CLIP_ERR_BAD_OPT, CLIP_PANIC)
/// \param[in] cmd Указатель на оригинальную структуру
/// \return Новый указатель на `command`, либо NULL при ошибке
__attribute__((malloc)) struct command *
copy_command(int *error, const struct command *cmd)
{
        if (NULL == cmd || NULL == cmd->dir || NULL == cmd->ext)
        {
                *error = CLIP_ERR_BAD_OPT;
                return NULL;
        }
        struct command *copy = malloc(sizeof(struct command));
        if (NULL == copy)
        {
                *error = CLIP_PANIC;
                return NULL;
        }
        copy->ext = strcopy(cmd->ext);
        copy->dir = strcopy(cmd->dir);
        if (NULL == copy->ext || NULL == copy->dir)
        {
                free((void *) copy->dir);
                free((void *) copy->ext);
                free(copy);
                *error = CLIP_PANIC;
                return NULL;
        }
        return copy;
}
