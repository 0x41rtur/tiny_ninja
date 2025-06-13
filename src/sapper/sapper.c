#include "sapper.h"

#include "clip/clip.h"
#include "common/common.h"
#include "common/console.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <unistd.h>

static char *
concat(const char *first, ...)
{
        va_list args;
        size_t  total_len = strlen(first);
        va_start(args, first);
        const char *s = NULL;
        while (NULL != (s = va_arg(args, const char*)))
        {
                total_len += strlen(s);
        }
        va_end(args);
        char *res = malloc(total_len + 1);
        if (NULL == res)
        {
                memory_error();
                exit(EXIT_FAILURE);
        }
        strcpy(res, first);
        va_start(args, first);
        while (NULL != (s = va_arg(args, char*)))
        {
                if (NULL == strcat(res, s))
                {
                        memory_error();
                        exit(EXIT_FAILURE);
                }
        }
        va_end(args);
        return res;
}

void
apply_tnt(struct tnt **tnt)
{
        size_t current = 0;
        while (NULL != tnt[current])
        {
                const struct tnt *cur   = tnt[current];
                const DIR *       exist = opendir(cur->cmd->dir);
                if (NULL == exist)
                {
                        if (-1 == mkdir(cur->cmd->dir, 0777))
                        {
                                make_directory_error(cur->cmd->dir);
                                goto next;
                        }
                }
                const char *new_path = concat(cur->cmd->dir, "/", cur->name,
                                              NULL);
                if (NULL == new_path)
                {
                        concat_error();
                        goto next;
                }
                if (access(new_path, F_OK) == 0)
                {
                        file_exists_error(new_path);
                        goto next;
                }
                if (-1 == rename(cur->name, new_path))
                {
                        rename_error(cur->name, new_path);
                        goto next;
                }
                move_file_success(cur->name, new_path);
        next:
                ++current;
        }
}
