#ifndef CLI_H
#define CLI_H

struct command
{
        const char *ext;
        const char *dir;
};

const struct command **
clip(int argc, char **argv);
struct command *
copy_command(const struct command *);

#endif //CLI_H
