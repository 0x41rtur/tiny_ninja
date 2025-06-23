#include "clip.h"
#include "executer.h"
#include "fs.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
usage(const char *prog_name);

int
main(const int argc, char **argv)
{
        int                    clip_error = CLIP_OK;
        const struct command **commands   = clip(&clip_error, argc, argv);
        if (commands == NULL || clip_error != CLIP_OK)
        {
                fprintf(stderr, "Ошибка разбора аргументов\n");
                usage(argv[0]);
                return EXIT_FAILURE;
        }
        for (const struct command **cmd = commands; cmd && *cmd; ++cmd)
        {
                struct target **targets = find_target(*cmd);

                if (targets == NULL)
                {
                        fprintf(stderr,
                                "Нет подходящих файлов с расширением: '%s'\n",
                                (*cmd)->ext);
                        continue;
                }
                for (struct target **t = targets; *t; ++t)
                {
                        int exec_error = EXECUTOR_OK;
                        if (execute(&exec_error, *t) == -1)
                        {
                                switch (exec_error)
                                {
                                case EXECUTOR_ERR_BAD_ARG:
                                        fprintf(stderr,
                                                "Некорректные аргументы\n");
                                        break;
                                case EXECUTOR_ERR_CREATE_PATH:
                                        fprintf(stderr, "Ошибка при создании "
                                                        "пути к файлу\n");
                                        break;
                                case EXECUTOR_ERR_FILE_EXISTS:
                                        fprintf(stderr,
                                                "Файл уже существует: %s/%s\n",
                                                (*t)->cmd->dir, (*t)->name);
                                        break;
                                case EXECUTOR_ERR_MV:
                                        fprintf(stderr,
                                                "Ошибка при перемещении файла: "
                                                "%s\n",
                                                (*t)->name);
                                        break;
                                default:
                                        fprintf(stderr, "Неизвестная ошибка\n");
                                        break;
                                }
                        }
                        else
                        {
                                printf("Успешно: %s → %s/%s\n", (*t)->name,
                                       (*t)->cmd->dir, (*t)->name);
                        }
                }
                for (struct target **t = targets; *t; ++t)
                {
                        free((void *) (*t)->name);
                        free(
                            (void *) (*t)
                                ->cmd); // предполагается, что copy_command() возвращает malloc'нутый cmd
                        free(*t);
                }
                free(targets);
        }
        for (const struct command **c = commands; *c; ++c)
        {
                free((void *) (*c)->ext);
                free((void *) (*c)->dir);
                free((void *) *c);
        }
        free(commands);
        return EXIT_SUCCESS;
}

void
usage(const char *prog_name)
{
        printf("Использование: %s [-e расширение -d директория] | [-m карта]\n",
               prog_name);
        printf("Опции:\n");
        printf("  -e <расширение>    Фильтрация файлов по расширению\n");
        printf("  -d <директория>    Каталог назначения\n");
        printf("  -m <карта>         Карта расширений и директорий (пример: "
               "\"jpg=images;mp4=videos\")\n");
        printf("  -h                 Показать это сообщение и выйти\n");
}
