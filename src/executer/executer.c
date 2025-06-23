#include "executer.h"

#include "clip.h"
#include "common.h"
#include "fs.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/// Выполняет перемещение целевого файла в указанную директорию,
/// создавая путь при необходимости.
///
/// Алгоритм работы:
/// 1. Проверяет, что передан ненулевой указатель на `target`.
/// 2. Создаёт директорию `target->cmd->dir` и все родительские, если их нет.
/// 3. Формирует полный путь назначения: `<dir>/<name>`.
/// 4. Если файл с таким именем уже существует в целевой директории — возвращает ошибку.
/// 5. Перемещает файл `target->name` в сформированный путь с помощью `rename()`.
///
/// Параметры:
/// - `error`: указатель на переменную, в которую будет записан код ошибки.
///            Возможные значения:
///              - `EXECUTOR_OK` — операция прошла успешно
///              - `EXECUTOR_ERR_BAD_ARG` — передан NULL или путь недопустим
///              - `EXECUTOR_ERR_CREATE_PATH` — ошибка при формировании строки пути
///              - `EXECUTOR_ERR_FILE_EXISTS` — файл в целевой директории уже существует
///              - `EXECUTOR_ERR_MV` — не удалось переместить файл
///
/// - `target`: указатель на структуру `struct target`,
///             содержащую имя файла и целевую директорию через `target->cmd->dir`.
///
/// Возвращает:
/// - `0`, если файл был успешно перемещён;
/// - `-1` при любой ошибке, подробности — в `*error`.
///
/// Примечания:
/// - Все пути считаются относительными от текущей рабочей директории.
/// - В случае ошибок освобождаются промежуточные ресурсы (строки).
/// - Файл `target->name` должен существовать до вызова, иначе `rename()` вернёт ошибку.
int
execute(int *error, const struct target *target)
{
        if (NULL == target)
        {
                *error = EXECUTOR_ERR_BAD_ARG;
                return -1;
        }
        *error           = EXECUTOR_OK;
        const int status = make_dir_recursive(target->cmd->dir);
        if (-1 == status)
        {
                *error = EXECUTOR_ERR_BAD_ARG;
                return -1;
        }
        const char *str = concat(target->cmd->dir, "/", target->name, NULL);
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
        if (-1 == rename(target->name, str))
        {
                *error = EXECUTOR_ERR_MV;
                free((void *) str);
                return -1;
        }
        return 0;
}
