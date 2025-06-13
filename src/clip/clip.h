#ifndef CLI_H
#define CLI_H

struct command
{
        char *ext;
        char *dir;
};

struct command *
clip(int argc, char **argv);
struct command *
copy_command(const struct command *);

#endif //CLI_H
