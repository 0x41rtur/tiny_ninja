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
                "  tn -e <extension> -d <directory> [-f] [-h]\n"
                "  tn -m \"<ext1>:<dir1>,<ext2>:<dir2>\" [-f] [-h]\n"
                "\n"
                "Опции:\n"
                "  -h                  Показать это руководство.\n"
                "  -e <extension>      Расширение файлов для перемещения (например: jpg, log).\n"
                "                      Обязательный параметр, если не используется -m.\n"
                "  -d <directory>      Целевая директория. Только относительные пути:\n"
                "                      example, /example, ./example, example/ — все считаются как ./example.\n"
                "                      Недопустимы пути, содержащие \"..\"\n"
                "                      Если директория не указана — будет создана папка с именем расширения.\n"
                "  -m <map>            Расширенный режим: несколько расширений и директорий.\n"
                "                      Формат: jpg=images;mp4=videos\n"
                "                      Если в пути присутствуем пробе то следует обернуть путь в \"\""
                "                      Каждое правило в виде <расширение>:<директория>\n"
                "  -f                  Пропустить все подтверждения и сразу выполнить.\n"
                "\n"
                "Примеры:\n"
                "  tn -e jpg -d images\n"
                "  tn -e log -d logs\n"
                "  tn -e png -d ./images -f\n"
                "  tn -m jpg=images;mp4=videos -f\n"
                "  tn -m jpg=\"space space\";mp4=videos -f\n"
                "\n"
                );

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
clip_m_error()
{
        fprintf(stderr,
                "\n❌ Ошибка: некорректный аргумент для опции `-m`\n"
                "\n"
                "🔧 Ожидаемый формат:\n"
                "    jpg:images,mp4:video\n"
                "\n"
                "📌 Каждое правило должно быть в формате <расширение>:<директория>,\n"
                "    без пробелов, только строчные или заглавные буквы и цифры.\n"
                "\n"
                "✅ Пример корректного использования:\n"
                "    tn -m jpg=images;mp4=video\"\n"
                "    tn -m jpg=\"img space path\";mp4=video\"\n"
                "\n"
                "🛑 Пример некорректного:\n"
                "    tn -m jpg = images; mp4 = video   ← ❌\n"
                "\n"
                "Для подробной справки: tn -h\n"
                );
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
no_matching_files_error(const char *ext)
{
        fprintf(stderr,
                "📁 В текущей директории не найдено файлов с расширением \"%s\".\n"
                "🔍 Проверьте путь и наличие нужных файлов.\n",
                ext);
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
