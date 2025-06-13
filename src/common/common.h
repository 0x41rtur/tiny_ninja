#ifndef COMMON_H
#define COMMON_H

struct tnt
{
        char *          name;
        struct command *cmd;
};

struct tnt **
find_tnt(const struct command *);
char *
strdup_my(const char *s);

#endif //COMMON_H
