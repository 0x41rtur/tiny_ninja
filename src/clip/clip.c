#include "clip.h"

#include "common/console.h"
#include "common/common.h"

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int
is_valid_extension(const char *ext)
{
        if (NULL == ext || 0 == strlen(ext))
        {
                return 0;
        }
        const char *ch = ext;
        while ('\0' != *ch)
        {
                if (0 == isalnum(*ch))
                {
                        return 0;
                }
                ++ch;
        }
        return 1;
}

static char *
normalize(const char *path)
{
        if (NULL == path)
        {
                return NULL;
        }
        while (isspace(*path))
        {
                ++path;
        }
        if ('/' == path[0] && 0 != isalnum(path[1]))
        {
                char *r = strdup_my(path + 1);
                return r;
        }
        if (strncmp(path, "./", 2) == 0 && 0 != isalnum(path[2]))
        {
                return strdup_my(path + 2);
        }
        if (strlen(path) == 0)
        {
                return NULL;
        }
        return strdup_my(path);
}

struct command *
clip(const int argc, char **argv)
{
        const char *extension = NULL;
        const char *directory = NULL;
        int force = 0;
        int opt   = 0;
        opterr = 0;
        while ((opt = getopt(argc, argv, "e:d:fh")) != -1)
        {
                switch (opt)
                {
                case 'e':
                        if (0 == is_valid_extension(optarg))
                        {
                                clip_e_error();
                                exit(EXIT_FAILURE);
                        }
                        extension = optarg;
                        break;
                case 'd':
                {
                        const char *normalized = normalize(optarg);
                        if (NULL == normalized)
                        {
                                clip_d_error();
                                exit(EXIT_FAILURE);
                        }
                        directory = normalized;
                        break;
                }
                case 'f':
                        force = 1;
                        break;
                default:
                        clip_info();
                        exit(EXIT_FAILURE);
                }
        }
        if (NULL != extension && NULL != directory)
        {
                struct command *cmd = malloc(sizeof(struct command));
                if (NULL == cmd)
                {
                        memory_error();
                        exit(EXIT_FAILURE);
                }
                cmd->ext = strdup_my(extension);
                cmd->dir = strdup_my(directory);
                if (NULL == cmd->dir || NULL == cmd->ext)
                {
                        memory_error();
                        exit(EXIT_FAILURE);
                }
                if (0 == force)
                {
                        clip_confirm_exit(extension, directory);
                }
                return cmd;
        }
        if (NULL == directory)
        {
                clip_d_error();
        }
        if (NULL == extension)
        {
                clip_e_error();
        }
        exit(EXIT_FAILURE);
}

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
        copy->ext = strdup_my(cmd->ext);
        copy->dir = strdup_my(cmd->dir);
        if (NULL == copy->ext || NULL == copy->dir)
        {
                memory_error();
                exit(EXIT_FAILURE);
        }
        return copy;
}
