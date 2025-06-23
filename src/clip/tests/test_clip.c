#include "test_clip.h"

#include "clip.h"

#include "unity.h"

#include <stdlib.h>

void
test_clip_invalid_empty_args(void)
{
        char                  *argv[] = {"app"};
        int                    error  = 0;
        const struct command **cmds   = clip(&error, 1, argv);
        TEST_ASSERT_NULL(cmds);
        TEST_ASSERT_EQUAL_INT(CLIP_ERR_BAD_OPT, error);
}

void
test_clip_invalid_ext_empty(void)
{
        char                  *argv[] = {"app", "-e", ".txt"};
        int                    error  = 0;
        const struct command **cmds   = clip(&error, 3, argv);
        TEST_ASSERT_NULL(cmds);
        TEST_ASSERT_EQUAL_INT(CLIP_ERR_BAD_E_OPT, error);
}

void
test_clip_invalid_dir_empty(void)
{
        char                  *argv[] = {"app", "-d", " "};
        int                    error  = 0;
        const struct command **cmds   = clip(&error, 3, argv);
        TEST_ASSERT_NULL(cmds);
        TEST_ASSERT_EQUAL_INT(CLIP_ERR_BAD_D_OPT, error);
}

void
test_clip_invalid_map_arg(void)
{
        char                  *argv[] = {"app", "-m",
                                         "mp4:/,jpg:"}; // например, некорректный маппинг
        int                    error  = 0;
        const struct command **cmds   = clip(&error, 3, argv);
        TEST_ASSERT_NULL(cmds);
        TEST_ASSERT_EQUAL_INT(CLIP_ERR_BAD_M_OPT, error);
}

void
test_clip_valid_single_pair(void)
{
        char                  *argv[] = {"app", "-e", "txt", "-d", "docs"};
        int                    error  = 0;
        const struct command **cmds   = clip(&error, 5, argv);
        TEST_ASSERT_NOT_NULL(cmds);
        TEST_ASSERT_NOT_NULL(cmds[0]);
        TEST_ASSERT_EQUAL_STRING("txt", cmds[0]->ext);
        TEST_ASSERT_EQUAL_STRING("docs", cmds[0]->dir);
        TEST_ASSERT_NULL(cmds[1]); // конец списка
}

void
test_clip_valid_mapping(void)
{
        char                  *argv[] = {"app", "-m", "jpg=images;mp4=videos"};
        int                    error  = 0;
        const struct command **cmds   = clip(&error, 3, argv);
        TEST_ASSERT_NOT_NULL(cmds);
        TEST_ASSERT_NOT_NULL(cmds[0]);
        TEST_ASSERT_EQUAL_STRING("jpg", cmds[0]->ext);
        TEST_ASSERT_EQUAL_STRING("images", cmds[0]->dir);

        TEST_ASSERT_NOT_NULL(cmds[1]);
        TEST_ASSERT_EQUAL_STRING("mp4", cmds[1]->ext);
        TEST_ASSERT_EQUAL_STRING("videos", cmds[1]->dir);

        TEST_ASSERT_NULL(cmds[2]); // конец списка
}

void
test_clip_valid_trailing_slash(void)
{
        char                  *argv[] = {"app", "-e", "txt", "-d", "docs/"};
        int                    error  = 0;
        const struct command **cmds   = clip(&error, 5, argv);
        TEST_ASSERT_NOT_NULL(cmds);
        TEST_ASSERT_EQUAL_STRING("txt", cmds[0]->ext);
        TEST_ASSERT_EQUAL_STRING("docs/", cmds[0]->dir);
        TEST_ASSERT_NULL(cmds[1]);
}

void
test_copy_command_null_cmd(void)
{
        int             err  = -123;
        struct command *copy = copy_command(&err, NULL);
        TEST_ASSERT_NULL(copy);
        TEST_ASSERT_EQUAL_INT(CLIP_ERR_BAD_OPT, err);
}

void
test_copy_command_null_ext(void)
{
        struct command  cmd  = {.dir = "dir", .ext = NULL};
        int             err  = -123;
        struct command *copy = copy_command(&err, &cmd);
        TEST_ASSERT_NULL(copy);
        TEST_ASSERT_EQUAL_INT(CLIP_ERR_BAD_OPT, err);
}

void
test_copy_command_null_dir(void)
{
        struct command  cmd  = {.dir = NULL, .ext = "txt"};
        int             err  = -123;
        struct command *copy = copy_command(&err, &cmd);
        TEST_ASSERT_NULL(copy);
        TEST_ASSERT_EQUAL_INT(CLIP_ERR_BAD_OPT, err);
}

void
test_copy_command_valid(void)
{
        struct command  cmd  = {.ext = "txt", .dir = "output"};
        int             err  = -123;
        struct command *copy = copy_command(&err, &cmd);

        TEST_ASSERT_NOT_NULL(copy);
        TEST_ASSERT_EQUAL_STRING("txt", copy->ext);
        TEST_ASSERT_EQUAL_STRING("output", copy->dir);
        TEST_ASSERT(copy->ext != cmd.ext); // Убедиться, что копия
        TEST_ASSERT(copy->dir != cmd.dir);

        free((void *) copy->ext);
        free((void *) copy->dir);
        free(copy);
}
