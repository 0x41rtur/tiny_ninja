#include "common.h"

#include "console.h"
#include "clip/clip.h"

#include <limits.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define RUN_DIR "."

#ifndef PATH_MAX
#define PATH_MAX 4096  // fallback, POSIX минимум
#endif

char *
strdup_my(const char *s)
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

static char *
get_ext(const char *file)
{
        if (NULL == file)
        {
                return NULL;
        }
        size_t tail = strlen(file) - 1;
        // file.
        if (file[tail] == '.')
        {
                return NULL;
        }
        --tail;
        // .file имя допусимо, но семантика данных файлов иная, не берем такие файлы
        while (0 < tail)
        {
                if (file[tail] == '.')
                {
                        return strdup_my(file + tail + 1);
                }
                --tail;
        }
        return NULL;
}

static int
is_file(const char *dir, const char *filename)
{
        struct stat st;
        char        full_path[PATH_MAX];
        const int   len = snprintf(full_path, sizeof(full_path), "%s/%s",
                                 dir,
                                 filename);
        if (len < 0 || (size_t) len >= sizeof(full_path))
        {
                return 0;
        }
        if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode))
        {
                return 1;
        }
        return 0;
}

struct tnt **
find_tnt(const struct command *cmd)
{
        DIR *current_dir = opendir(RUN_DIR);
        if (NULL == current_dir)
        {
                open_dir_error(RUN_DIR);
                exit(EXIT_FAILURE);
        }
        size_t               count    = 0;
        size_t               capacity = 1;
        struct tnt **        entries  = NULL;
        const struct dirent *entry    = NULL;
        while (NULL != (entry = readdir(current_dir)))
        {
                if (1 == is_file(RUN_DIR, entry->d_name))
                {
                        {
                                char *ext = get_ext(entry->d_name);
                                if (NULL == ext)
                                {
                                        continue;
                                }
                                if (0 != strcmp(ext, cmd->ext))
                                {
                                        free(ext);
                                        continue;
                                }
                                free(ext);
                                if (NULL == entries || count == capacity)
                                {
                                        capacity *= 2;
                                        entries = realloc(
                                                entries,
                                                sizeof(struct dirent *) *
                                                capacity);
                                        if (NULL == entries)
                                        {
                                                memory_error();
                                                exit(EXIT_FAILURE);
                                        }
                                }
                                struct tnt *target = malloc(sizeof(struct tnt));
                                if (NULL == target)
                                {
                                        for (size_t i = 0; i < count; ++i)
                                        {
                                                free(entries[i]->name);
                                                free(entries[i]->cmd);
                                                free(entries[i]);
                                        }
                                        memory_error();
                                        exit(EXIT_FAILURE);
                                }
                                target->name = strdup_my(entry->d_name);
                                target->cmd  = copy_command(cmd);
                                if (NULL == target->cmd || NULL == target->name)
                                {
                                        for (size_t i = 0; i < count; ++i)
                                        {
                                                free(entries[i]->name);
                                                free(entries[i]->cmd);
                                                free(entries[i]);
                                        }
                                        free(entries);
                                        free(target->name);
                                        free(target->cmd);
                                        free(target);
                                        memory_error();
                                        exit(EXIT_FAILURE);
                                }
                                entries[count] = target;
                                ++count;
                        }
                }
        }
        if (NULL == entries || 0 == count)
        {
                char cwd[PATH_MAX] = {'/'};
                getcwd(cwd, sizeof(cwd));
                no_matching_files_error(cmd->ext, cwd);
                exit(EXIT_SUCCESS);
        }
        entries = realloc(entries,
                          sizeof(struct tnt *) * (count + 1));
        entries[count] = NULL;
        return entries;
}
