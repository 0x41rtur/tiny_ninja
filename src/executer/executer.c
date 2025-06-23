#include "executer.h"

#include "clip.h"
#include "common.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
execute(int* error, const struct tnt *tnt)
{
        if (NULL == tnt)
        {
                *error = EXECUTOR_ERR_BAD_ARG;
                return -1;
        }
        *error = EXECUTOR_OK;
        const int status = make_dir_recursive(tnt->cmd->dir);
        if (-1 == status)
        {
                *error = EXECUTOR_ERR_BAD_ARG;
                return -1;
        }
        const char *str = concat(tnt->cmd->dir, "/", tnt->name, NULL);
        if (NULL == str)
        {
                *error = EXECUTOR_ERR_CREATE_PATH;
                return -1;
        }
        if (access(str, F_OK) == 0)
        {
                *error = EXECUTOR_ERR_FILE_EXISTS;
                free((void *) str);
                return -1;
        }
        if (-1 == rename(tnt->name, str))
        {
                *error = EXECUTOR_ERR_MV;
                free((void *) str);
                return -1;
        }
        return 0;
}
