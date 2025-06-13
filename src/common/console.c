#include "console.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
clip_info(void)
{
        printf(
                "Использование:\n"
                "  tn -e <extension> -d <directory> [-f] [-h]\n\n"

                "Опции:\n"
                "  -h               Данное руководство."
                "  -e <extension>   Расширение файлов для перемещения (например, jpg, log).\n"
                "                   Обязательный параметр.\n"
                "  -d <directory>   Целевая директория для перемещения файлов.\n"
                "                   Поддерживаются только относительные пути:\n"
                "                   example, /example, ./example, example/ — все считаются как ./example.\n"
                "                   \"example with space\" - пути с пробелами следует обрамлять в \"\""
                "                   Недопустимо использовать пути с \"..\" (выход за пределы).\n"
                "                   Обязательный параметр.\n"
                "  -f               Игнорировать все подтверждения и выполнить сразу.\n\n"

                "Примеры:\n"
                "  tn -e jpg -d images\n"
                "  tn -e log -d logs\n"
                "  tn -e png -d ./images -f\n\n");
}

void
panic_unexpected(void)
{
        fprintf(stderr,
                "\n"
                "🚨 Ой! Что-то пошло не так.\n"
                "Возможно, вы нашли баг или что-то непредвиденное произошло.\n"
                "Пожалуйста, сообщите разработчику с описанием:\n"
                "  • Что вы пытались сделать\n"
                "  • Какие аргументы передавали\n"
                "  • Что вывел терминал\n\n"
                "🔧 Спасибо за помощь в улучшении Tiny Ninja!\n\n");
}

void
clip_confirm_exit(const char *ext, const char *dir)
{
        printf("Все файлы с расширением %s будут перемещены в \"%s\"\n\n", ext,
               dir);
        printf("Продолжить? Y\\N\r\n");
        char buf[16];
        if (fgets(buf, sizeof(buf), stdin) != NULL)
        {
                // сравнить buf с "н" или "Н" как строку
                if (strcmp(buf, "н\n") != 0 && strcmp(buf, "Н\n") != 0 &&
                    strcmp(buf, "y\n") != 0 && strcmp(buf, "Y\n") != 0)
                {
                        clip_info();
                        exit(EXIT_SUCCESS);
                }
        }
}

void
clip_e_error()
{
        fprintf(stderr,
                "❌ Ошибка: некорректное значение расширения:\n\n"
                "📌 Допустим только формат вроде \"jpg\", без точки, пробелов и символов.\n"
                "🔤 Учитывается регистр: \"JPG\" и \"jpg\" — это разные расширения.\n\n"
                "💡 Пример корректного ввода:\n"
                "    tn -e jpg -d backup\n");
}

void
clip_d_error()
{
        fprintf(stderr,
                "❌ Ошибка: недопустимый путь к директории:\n\n"
                "📁 Аргумент `-d` должен указывать на директорию — относительный путь внутри проекта.\n\n"
                "✅ Допустимые формы:\n"
                "   •  example\n"
                "   •  /example\n"
                "   •  ./example\n"
                "   •  example/\n"
                "   •  \"name with space\""
                "   Все они интерпретируются как относительный путь: ./example\n\n"
                "🚫 Недопустимо:\n"
                "   •  ../example   — выход за пределы текущей директории запрещён\n");
}

void
memory_error(void)
{
        fprintf(stderr,
                "🧠 Недостаточно памяти для продолжения работы.\n"
                "❌ Программа завершена. Попробуйте закрыть другие приложения и запустить снова.\n");
}

void
make_directory_error(const char *path)
{
        fprintf(stderr,
                "📁 Не удалось создать папку \"%s\".\n"
                "❌ Причина: %s\n", path, strerror(errno));
}

void
concat_error(void)
{
        fprintf(stderr,
                "🧵 Не удалось объединить строки — недостаточно памяти.\n"
                "❌ Программа завершена. Попробуйте закрыть другие приложения и повторить попытку.\n");
}

void
rename_error(const char *src, const char *dst)
{
        fprintf(stderr,
                "📦 Не удалось переместить файл:\n"
                "   из: %s\n"
                "   в : %s\n"
                "❌ Причина: %s\n",
                src, dst, strerror(errno));
}

void
file_exists_error(const char *path)
{
        fprintf(stderr,
                "📄 Файл \"%s\" уже существует и не может быть перезаписан.\n"
                "🔒 Выберите другое имя или удалите существующий файл.\n",
                path);
        exit(EXIT_FAILURE);
}

void
no_matching_files_error(const char *ext, const char *dir)
{
        fprintf(stderr,
                "📁 В директории \"%s\" не найдено файлов с расширением \"%s\".\n"
                "🔍 Проверьте путь и наличие нужных файлов.\n",
                dir, ext);
}

void
open_dir_error(const char *path)
{
        fprintf(stderr,
                "🚫 Ошибка: не удалось открыть директорию \"%s\"\n"
                "📄 Причина: %s\n"
                "🔍 Проверьте, существует ли путь, и есть ли у вас права доступа к директории.\n\n",
                path, strerror(errno));
}

void
move_file_success(const char *src, const char *dst)
{
        printf("📁 Файл \"%s\" успешно перемещён в \"%s\"\n", src, dst);
}
