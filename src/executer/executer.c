/**
* @file executer.c
 * @brief Исполняет команды для перемежения
 */

#include "executer.h"

#include "clip/clip.h"
#include "common/common.h"
#include "common/console.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Перемещает файлы в указанные директории согласно массиву tnt.
 *
 * Для каждой структуры `tnt`:
 * - Создаёт директорию `cmd->dir`, если она ещё не существует.
 * - Формирует новый путь как `dir/filename`.
 * - Проверяет, существует ли файл по новому пути.
 * - Если нет — выполняет `rename` (перемещение файла).
 * - Успешные и неудачные действия сопровождаются соответствующими сообщениями.
 *
 * Атомарность: при первой же ошибке создания директории — выполнение прерывается.
 * Это исключает частичное выполнение и упрощает откат (rollback).
 *
 * @param tnt Массив указателей на `struct tnt`, заканчивающийся `NULL`.
 * @return 0 при полном успехе, -1 при ошибке.
 *
 * @warning Если в массиве 20 файлов, и на 10-й директории произошла ошибка —
 *          остальные 10 файлов не будут перемещены. Это преднамеренное поведение.
 */
int
execute(struct tnt **tnt)
{
        if (NULL == tnt)
        {
                return -1;
        }
        struct tnt **current = tnt;
        while (NULL != *current)
        {
                const int status = mk_dir((*current)->cmd->dir);
                if (-1 == status)
                {
                        make_directory_error((*current)->cmd->dir);
                        ++current;
                        continue;
                }
                const char *str = concat((*current)->cmd->dir, "/",
                                         (*current)->name, NULL);
                if (NULL == str)
                {
                        concat_error();
                        if (0 == status)
                        {
                                rm_dir((*current)->cmd->dir);
                        }
                        return -1;
                }
                if (access(str, F_OK) == 0)
                {
                        file_exists_error(str);
                        ++current;
                        free((void *) str);
                        continue;
                }
                if (-1 == rename((*current)->name, str))
                {
                        rename_error((*current)->name, str);
                        ++current;
                        free((void *) str);
                        if (0 == status)
                        {
                                rm_dir((*current)->cmd->dir);
                        }
                        continue;
                }
                move_file_success((*current)->name, str);
                ++current;
                free((void *) str);
        }
        return 0;
}
