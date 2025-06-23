#ifndef CLI_H
#define CLI_H

struct command
{
        const char *ext;
        const char *dir;
};

enum clip_error
{
        CLIP_OK,
        CLIP_ERR_BAD_OPT,
        CLIP_ERR_BAD_E_OPT,
        CLIP_ERR_BAD_D_OPT,
        CLIP_ERR_BAD_M_OPT,
        CLIP_PANIC,
        CLIP_UNEXPECTED_OPT
};

const struct command **
clip(int *error, int argc, char **argv);
struct command *
copy_command(int *error, const struct command *);

#endif //CLI_H
