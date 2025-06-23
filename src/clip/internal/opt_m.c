#include "opt_m.h"

#include "opt_d.h"
#include "opt_e.h"
#include "clip.h"
#include "common.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define PAIR_DELIM      ';'  /// Разделитель пар (extension=directory;...)
#define KEY_VALUE_DELIM '='  /// Разделитель ключа и значения (расширение=директория)

/// Разбирает строку с маппингами расширений на директории.
///
/// Строка `arg` должна быть в формате:
///     "ext1=dir1;ext2=dir2;..."
///
/// Каждая пара проверяется с помощью валидаторов `arge` и `argd`.
/// При успехе возвращается массив `struct command*`, оканчивающийся `NULL`.
///
/// \param[out] error Код ошибки (ARGM_OK, ARGM_NULL_ARG, ARGM_INVALID_ARG, ARGM_MEM_ERR)
/// \param[in]  arg Строка с парами "расширение=директория"
/// \return NULL в случае ошибки, иначе массив команд (NULL-терминированный)
__attribute__((malloc)) const struct command **
argm(int *error, const char *arg)
{
        *error = ARGM_OK;
        if (NULL == arg)
        {
                *error = ARGM_NULL_ARG;
                return NULL;
        }
        const struct command **cmds     = NULL;
        size_t           count    = 0;
        size_t           capacity = 1;
        char** arr_tok = strtokarr(arg, PAIR_DELIM);
        if (NULL == arr_tok)
        {
                *error = ARGM_MEM_ERR;
                return NULL;
        }
        while (NULL != *arr_tok)
        {
                char *e = NULL;
                char *d = NULL;
                if (-1 == split(*arr_tok, &e, &d, KEY_VALUE_DELIM) ||
                    -1 == arge(e) || -1 == argd(d))
                {
                        free(e);
                        free(d);
                        *error = ARGM_INVALID_ARG;
                        return NULL;
                }
                struct command *cmd = malloc(sizeof(struct command));
                if (NULL == cmd)
                {
                        goto cleanup;
                        *error = ARGM_MEM_ERR;
                        return NULL;
                }
                cmd->ext = e;
                cmd->dir = d;
                if (count == capacity || NULL == cmds)
                {
                        capacity *= 2;
                        cmds =
                            realloc(cmds, capacity * sizeof(struct command *));
                        if (NULL == cmds)
                        {
                                goto cleanup;
                                *error = ARGM_MEM_ERR;
                                return NULL;
                        }
                }
                cmds[count] = cmd;
                ++count;
                ++arr_tok;
                continue;
        cleanup:
                free(e);
                free(d);
                free(cmd);
                return NULL;
        }
        if (0 == count)
        {
                *error = ARGM_INVALID_ARG;
                return NULL;
        }
        const struct command **new_cmds =
            realloc(cmds, (count + 1) * sizeof(struct command *));
        if (NULL == new_cmds)
        {
                while (NULL != *cmds)
                {
                        const struct command *cmd = *cmds;
                        free((void *) (*cmds)->dir);
                        free((void *) (*cmds)->dir);
                        ++cmds;
                        free((void *)cmd);
                }
                *error = ARGM_MEM_ERR;
                return NULL;
        }
        cmds        = new_cmds;
        cmds[count] = NULL;
        return cmds;
}
